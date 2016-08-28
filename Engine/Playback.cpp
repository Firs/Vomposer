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
#include "Playback.h"

#include <QAudioOutput>
#include <QCoreApplication>

#include "Sound.h"
#include "Utils.h"

FPlayback::FPlayback(QObject* Parent)
    : QObject(Parent)
    , AudioState(QAudio::StoppedState)
    , AvailableAudioOutputDevices(
          QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    , AudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice())
    , AudioOutput(nullptr)
{
    Initialize();
}

void FPlayback::Reset()
{
    Stop();

    AudioState = QAudio::StoppedState;
    AudioFormat = QAudioFormat();

    AudioOutputBuffer.close();
    AudioOutput.reset();
}

void FPlayback::PlaySound(const FSound& Sound)
{
    Stop();
    AudioOutputBuffer.setBuffer(const_cast<QByteArray*>(&Sound.Data));
    Start();
}

void FPlayback::Start()
{
    if (AudioOutput)
    {
        if (AudioState == QAudio::SuspendedState)
        {
#ifdef Q_OS_WIN
            // The Windows backend seems to internally go back into ActiveState
            // while still returning SuspendedState, so to ensure that it doesn't
            // ignore the resume() call, we first re-suspend
            AudioOutput->suspend();
#endif
            AudioOutput->resume();
        }
        else
        {
            CHECKED_CONNECT(AudioOutput.data(), SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(OnAudioStateChanged(QAudio::State)));
            CHECKED_CONNECT(AudioOutput.data(), SIGNAL(notify()),
                            this, SLOT(OnAudioNotify()));

            AudioOutputBuffer.close();
            AudioOutputBuffer.open(QIODevice::ReadOnly);
            AudioOutput->start(&AudioOutputBuffer);
        }
    }
}

void FPlayback::Pause()
{
    if (QAudio::ActiveState == AudioState ||
        QAudio::IdleState == AudioState)
    {
        AudioOutput->suspend();
    }
}

void FPlayback::Stop()
{
    if (AudioOutput)
    {
        AudioOutput->stop();
        //QCoreApplication::instance()->processEvents();
        AudioOutput->disconnect();
        AudioOutputBuffer.close();

    }
}


void FPlayback::SetAudioOutputDevice(const QAudioDeviceInfo& Device)
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

void FPlayback::OnAudioStateChanged(QAudio::State State)
{
    qDebug() << "FEngine::audioStateChanged from" << AudioState
                 << "to" << State;

    if (QAudio::IdleState == State)
    {
        Stop();
    }
    else if (QAudio::StoppedState == State)
    {
        if (AudioOutput->error() != QAudio::NoError)
        {
            Reset();
            return;
        }
        AudioState = State;
    }
}

void FPlayback::OnAudioNotify()
{

}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

bool FPlayback::Initialize()
{
    bool Result = false;

    QAudioFormat Format = AudioFormat;

    if (SelectAudioFormat())
    {
        if (Format != AudioFormat)
        {
            AudioOutput.reset(new QAudioOutput(AudioDeviceInfo, AudioFormat, this));
            AudioOutput->setNotifyInterval(30);
            Result = true;
        }
    }

    qDebug() << "FEngine::Initialize()";
    qDebug() << "AudioFormat" << AudioFormat;

    return Result;
}

bool FPlayback::SelectAudioFormat()
{
    bool bFoundSupportedFormat = false;

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
        AudioFormat = QAudioFormat();
    }

    AudioFormat = Format;

    return bFoundSupportedFormat;
}


