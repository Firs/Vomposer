/**************************************************************************
** Copyright (c) 2016 Fedor Eliseev <feliseev@gmail.com>.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, version 3.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Lesser Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#include "PitchMonitor.h"

#include <QAudioInput>
#include <QCoreApplication>
#include <QDebug>
#include <QSet>
#include <QList>
#include <QMetaObject>

#include "PitchTracker.h"
#include "Utils.h"

const int BufferDurationUs = 1 * 1000000;
const int NotifyIntervalMs = 20;

FPitchMonitor::FPitchMonitor(QObject *Parent)
    : QObject(Parent)
    , CurrentAudioState(QAudio::StoppedState)
    , AvailableAudioInputDevices(
          QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    , AudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice())
    , AudioInput(nullptr)
    , AudioInputIODevice(nullptr)
    , AudioBufferLength(0)
{
    Initialize();
}

//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------

void FPitchMonitor::Start()
{
    if (AudioInput)
    {
        if (CurrentAudioState == QAudio::SuspendedState)
        {
            AudioInput->resume();
        }
        else
        {
            AudioBuffer.fill(0);

            CHECKED_CONNECT(AudioInput, SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(OnAudioStateChanged(QAudio::State)));

            AudioInputIODevice = AudioInput->start();

            CHECKED_CONNECT(AudioInputIODevice, SIGNAL(readyRead()),
                            this, SLOT(OnAudioDataReady()));
        }
    }
}

void FPitchMonitor::Pause()
{
    if (QAudio::ActiveState == CurrentAudioState ||
        QAudio::IdleState == CurrentAudioState)
    {
        AudioInput->suspend();
    }
}

void FPitchMonitor::SetAudioInputDevice(const QAudioDeviceInfo& Device)
{
    if (AudioDeviceInfo.deviceName() != Device.deviceName())
    {
        AudioDeviceInfo = Device;
        Initialize();
    }
}

//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void FPitchMonitor::OnAudioStateChanged(QAudio::State State)
{
    qDebug() << "FEngine::audioStateChanged from" << CurrentAudioState
                 << "to" << State;

    if (QAudio::StoppedState == State)
    {
        if (AudioInput->error() != QAudio::NoError)
        {
            Reset();
            return;
        }
    }
    SetAudioState(State);
}

void FPitchMonitor::OnAudioDataReady()
{
    const qint64 BytesToRead = AudioInput->bytesReady();
    Q_ASSERT(BytesToRead <= AudioBuffer.size());
    const qint64 BytesRead = AudioInputIODevice->read(
                AudioBuffer.data(), BytesToRead);
    if (BytesRead > 0)
    {
        const qint16* BufferPtr = reinterpret_cast<const qint16*>(
                    AudioBuffer.constData());
        const int NumSamplesRead = BytesRead / (2 * CurrentAudioFormat.channelCount());
        for (int Idx = 0; Idx < NumSamplesRead; ++Idx)
        {
            qreal Value = PcmToReal(*BufferPtr);
            RecordedSamples[Idx] = Value;
            BufferPtr += CurrentAudioFormat.channelCount();
        }

        PitchTracker.Update(RecordedSamples.data(), 0, NumSamplesRead);

        auto Frequency = PitchTracker.GetCurrentFrequency();
        auto Pitch = FPitch::FromFrequency(Frequency);
        emit PitchDetected(Pitch, Frequency);
    }
}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

void FPitchMonitor::ResetAudioDevices()
{
    delete AudioInput;
    AudioInput = nullptr;
    AudioInputIODevice = nullptr;
}

void FPitchMonitor::Reset()
{
    Stop();

    SetAudioState(QAudio::StoppedState);
    SetAudioFormat(QAudioFormat());

    AudioBuffer.clear();
    AudioBufferLength = 0;

    RecordedSamples.clear();

    ResetAudioDevices();
}

bool FPitchMonitor::Initialize()
{
    bool Result = false;

    QAudioFormat Format = CurrentAudioFormat;

    if (SelectAudioFormat())
    {
        if (CurrentAudioFormat != Format)
        {
            ResetAudioDevices();

            AudioBufferLength = CalculateAudioLength(CurrentAudioFormat, BufferDurationUs);
            AudioBuffer.resize(AudioBufferLength);
            AudioBuffer.fill(0);
            RecordedSamples.resize(AudioBufferLength / (2 * CurrentAudioFormat.channelCount()));

            AudioInput = new QAudioInput(AudioDeviceInfo, CurrentAudioFormat, this);
            AudioInput->setNotifyInterval(NotifyIntervalMs);
            Result = true;
        }
    }

    qDebug() << "FEngine::Initialize()";
    qDebug() << "AudioFormat" << CurrentAudioFormat;

    return Result;
}

bool FPitchMonitor::SelectAudioFormat()
{
    bool bFoundSupportedFormat = false;

    QList<int> SampleRatesList;
#ifdef Q_OS_WIN
    // The Windows audio backend does not correctly report format support
    // (see QTBUG-9100).  Furthermore, although the audio subsystem captures
    // at 11025Hz, the resulting audio is corrupted.
    SampleRatesList += 8000;
#endif

    SampleRatesList += AudioDeviceInfo.supportedSampleRates();
    SampleRatesList = SampleRatesList.toSet().toList(); // remove duplicates
    qSort(SampleRatesList);
    qDebug() << "FEngine::Initialize SampleRatesList" << SampleRatesList;

    QList<int> ChannelsList;
    ChannelsList += AudioDeviceInfo.supportedChannelCounts();
    ChannelsList = ChannelsList.toSet().toList();
    qSort(ChannelsList);
    qDebug() << "FEngine::Initialize ChannelsList" << ChannelsList;

    QAudioFormat Format;
    Format.setByteOrder(QAudioFormat::LittleEndian);
    Format.setCodec("audio/pcm");
    Format.setSampleSize(16);
    Format.setSampleType(QAudioFormat::SignedInt);
    Format.setSampleRate(44100);
    Format.setChannelCount(1);

    bFoundSupportedFormat = AudioDeviceInfo.isFormatSupported(Format);
    qDebug() << "FEngine::Initialize checking " << Format << bFoundSupportedFormat;

    if (!bFoundSupportedFormat)
    {
        Format = QAudioFormat();
    }

    SetAudioFormat(Format);

    return bFoundSupportedFormat;
}

void FPitchMonitor::Stop()
{
    if (AudioInput)
    {
        AudioInput->stop();
        QCoreApplication::instance()->processEvents();
        AudioInput->disconnect();
    }
    AudioInputIODevice = nullptr;
}

void FPitchMonitor::SetAudioState(QAudio::State State)
{
    CurrentAudioState = State;
}

void FPitchMonitor::SetAudioFormat(const QAudioFormat& Format)
{
    const bool bChanged = (Format != CurrentAudioFormat);
    CurrentAudioFormat = Format;
    AudioBufferLength = CalculateAudioLength(CurrentAudioFormat, BufferDurationUs);
    if (bChanged)
    {
        emit AudioFormatChanged(CurrentAudioFormat);
    }
}
