/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "Engine.h"
#include <3rdParty/utils.h>
#include <3rdParty/wavfile.h>

#include <math.h>

#include <QAudioInput>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QMetaObject>
#include <QSet>
#include <QThread>

#include "PitchTracker.h"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

const qint64 BufferDurationUs       = 10 * 1000000;
const int    NotifyIntervalMs       = 100;

// Size of the level calculation window in microseconds
const int    LevelWindowUs          = 0.1 * 1000000;

// Waveform window size in microseconds
const qint64 WaveformWindowDuration = 500 * 1000;

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

FEngine::FEngine(QObject *parent)
    :   QObject(parent)
    ,   m_mode(QAudio::AudioInput)
    ,   m_state(QAudio::StoppedState)
    ,   m_file(0)
    ,   m_analysisFile(0)
    ,   m_availableAudioInputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    ,   m_audioInputDevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInput(0)
    ,   m_audioInputIODevice(0)
    ,   m_recordPosition(0)
    ,   m_availableAudioOutputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    ,   m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_playPosition(0)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_dataLength(0)
    ,   m_levelBufferLength(0)
    ,   m_rmsLevel(0.0)
    ,   m_peakLevel(0.0)
    ,   m_count(0)
{
    initialize();

#ifdef DUMP_DATA
    createOutputDir();
#endif
}

FEngine::~FEngine()
{

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

bool FEngine::loadFile(const QString &fileName)
{
    reset();
    bool result = false;
    Q_ASSERT(!m_file);
    Q_ASSERT(!fileName.isEmpty());
    m_file = new WavFile(this);
    if (m_file->open(fileName)) {
        if (isPCMS16LE(m_file->fileFormat())) {
            result = initialize();
        } else {
            emit errorMessage(tr("Audio format not supported"),
                              formatToString(m_file->fileFormat()));
        }
    } else {
        emit errorMessage(tr("Could not open file"), fileName);
    }
    if (result) {
        m_analysisFile = new WavFile(this);
        m_analysisFile->open(fileName);
    }
    return result;
}

bool FEngine::initializeRecord()
{
    reset();
    ENGINE_DEBUG << "FEngine::initializeRecord";
    Q_ASSERT(!m_file);
    return initialize();
}

qint64 FEngine::bufferLength() const
{
    return m_file ? m_file->size() : m_bufferLength;
}


//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------

void FEngine::startRecording()
{
    if (m_audioInput) {
        if (QAudio::AudioInput == m_mode &&
            QAudio::SuspendedState == m_state) {
            m_audioInput->resume();
        } else {
            m_buffer.fill(0);
            setRecordPosition(0, true);
            stopPlayback();
            m_mode = QAudio::AudioInput;
            CHECKED_CONNECT(m_audioInput, SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(audioStateChanged(QAudio::State)));
            CHECKED_CONNECT(m_audioInput, SIGNAL(notify()),
                            this, SLOT(audioNotify()));
            m_count = 0;
            m_dataLength = 0;
            emit dataLengthChanged(0);
            m_audioInputIODevice = m_audioInput->start();
            CHECKED_CONNECT(m_audioInputIODevice, SIGNAL(readyRead()),
                            this, SLOT(audioDataReady()));
        }
    }
}

void FEngine::startPlayback()
{
    if (m_audioOutput) {
        if (QAudio::AudioOutput == m_mode &&
            QAudio::SuspendedState == m_state) {
#ifdef Q_OS_WIN
            // The Windows backend seems to internally go back into ActiveState
            // while still returning SuspendedState, so to ensure that it doesn't
            // ignore the resume() call, we first re-suspend
            m_audioOutput->suspend();
#endif
            m_audioOutput->resume();
        } else {
            setPlayPosition(0, true);
            stopRecording();
            m_mode = QAudio::AudioOutput;
            CHECKED_CONNECT(m_audioOutput, SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(audioStateChanged(QAudio::State)));
            CHECKED_CONNECT(m_audioOutput, SIGNAL(notify()),
                            this, SLOT(audioNotify()));
            m_count = 0;
            if (m_file) {
                m_file->seek(0);
                m_bufferPosition = 0;
                m_dataLength = 0;
                m_audioOutput->start(m_file);
            } else {
                m_audioOutputIODevice.close();
                m_audioOutputIODevice.setBuffer(&m_buffer);
                m_audioOutputIODevice.open(QIODevice::ReadOnly);
                m_audioOutput->start(&m_audioOutputIODevice);
            }
        }
    }
}

void FEngine::suspend()
{
    if (QAudio::ActiveState == m_state ||
        QAudio::IdleState == m_state) {
        switch (m_mode) {
        case QAudio::AudioInput:
            m_audioInput->suspend();
            break;
        case QAudio::AudioOutput:
            m_audioOutput->suspend();
            break;
        }
    }
}

void FEngine::setAudioInputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioInputDevice.deviceName()) {
        m_audioInputDevice = device;
        initialize();
    }
}

void FEngine::setAudioOutputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioOutputDevice.deviceName()) {
        m_audioOutputDevice = device;
        initialize();
    }
}


//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void FEngine::audioNotify()
{
    switch (m_mode) {
    case QAudio::AudioInput: {
            const qint64 recordPosition = qMin(m_bufferLength, audioLength(m_format, m_audioInput->processedUSecs()));
            setRecordPosition(recordPosition);
            const qint64 levelPosition = m_dataLength - m_levelBufferLength;
            if (levelPosition >= 0)
                calculateLevel(levelPosition, m_levelBufferLength);
            emit bufferChanged(0, m_dataLength, m_buffer);
        }
        break;
    case QAudio::AudioOutput: {
            const qint64 playPosition = audioLength(m_format, m_audioOutput->processedUSecs());
            setPlayPosition(qMin(bufferLength(), playPosition));
            const qint64 levelPosition = playPosition - m_levelBufferLength;
            if (m_file) {
                if (levelPosition > m_bufferPosition ||
                    m_levelBufferLength > m_dataLength) {
                    m_bufferPosition = 0;
                    m_dataLength = 0;
                    // Data needs to be read into m_buffer in order to be analysed
                    const qint64 readPos = qMax(qint64(0), levelPosition);
                    const qint64 readEnd = qMin(m_analysisFile->size(), levelPosition + m_levelBufferLength);
                    const qint64 readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
                    qDebug() << "FEngine::audioNotify [1]"
                             << "analysisFileSize" << m_analysisFile->size()
                             << "readPos" << readPos
                             << "readLen" << readLen;
                    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
                        m_buffer.resize(readLen);
                        m_bufferPosition = readPos;
                        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
                        qDebug() << "FEngine::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
                    } else {
                        qDebug() << "FEngine::audioNotify [2]" << "file seek error";
                    }
                    emit bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
                    //PitchTracker.Update(m_buffer);
                }
            } else {
                if (playPosition >= m_dataLength)
                    stopPlayback();
            }
            if (levelPosition >= 0 && levelPosition + m_levelBufferLength < m_bufferPosition + m_dataLength)
                calculateLevel(levelPosition, m_levelBufferLength);
        }
        break;
    }
}

void FEngine::audioStateChanged(QAudio::State state)
{
    ENGINE_DEBUG << "FEngine::audioStateChanged from" << m_state
                 << "to" << state;

    if (QAudio::IdleState == state && m_file && m_file->pos() == m_file->size()) {
        stopPlayback();
    } else {
        if (QAudio::StoppedState == state) {
            // Check error
            QAudio::Error error = QAudio::NoError;
            switch (m_mode) {
            case QAudio::AudioInput:
                error = m_audioInput->error();
                break;
            case QAudio::AudioOutput:
                error = m_audioOutput->error();
                break;
            }
            if (QAudio::NoError != error) {
                reset();
                return;
            }
        }
        setState(state);
    }
}

void FEngine::audioDataReady()
{
    Q_ASSERT(0 == m_bufferPosition);
    const qint64 bytesReady = m_audioInput->bytesReady();
    const qint64 bytesSpace = m_buffer.size() - m_dataLength;
    const qint64 bytesToRead = qMin(bytesReady, bytesSpace);

    const qint64 bytesRead = m_audioInputIODevice->read(
                                       m_buffer.data() + m_dataLength,
                                       bytesToRead);

    if (bytesRead) {
        m_dataLength += bytesRead;
        emit dataLengthChanged(dataLength());
    }

    if (m_buffer.size() == m_dataLength)
        stopRecording();
}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

void FEngine::resetAudioDevices()
{
    delete m_audioInput;
    m_audioInput = 0;
    m_audioInputIODevice = 0;
    setRecordPosition(0);
    delete m_audioOutput;
    m_audioOutput = 0;
    setPlayPosition(0);
    setLevel(0.0, 0.0, 0);
}

void FEngine::reset()
{
    stopRecording();
    stopPlayback();
    setState(QAudio::AudioInput, QAudio::StoppedState);
    setFormat(QAudioFormat());
    delete m_file;
    m_file = 0;
    delete m_analysisFile;
    m_analysisFile = 0;
    m_buffer.clear();
    m_bufferPosition = 0;
    m_bufferLength = 0;
    m_dataLength = 0;
    emit dataLengthChanged(0);
    resetAudioDevices();
}

bool FEngine::initialize()
{
    bool result = false;

    QAudioFormat format = m_format;

    if (selectFormat()) {
        if (m_format != format) {
            resetAudioDevices();
            if (m_file) {
                emit bufferLengthChanged(bufferLength());
                emit dataLengthChanged(dataLength());
                emit bufferChanged(0, 0, m_buffer);
                setRecordPosition(bufferLength());
                result = true;
            } else {
                m_bufferLength = audioLength(m_format, BufferDurationUs);
                m_buffer.resize(m_bufferLength);
                m_buffer.fill(0);
                emit bufferLengthChanged(bufferLength());
                emit bufferChanged(0, 0, m_buffer);
                m_audioInput = new QAudioInput(m_audioInputDevice, m_format, this);
                m_audioInput->setNotifyInterval(NotifyIntervalMs);
                result = true;
            }
            m_audioOutput = new QAudioOutput(m_audioOutputDevice, m_format, this);
            m_audioOutput->setNotifyInterval(NotifyIntervalMs);
        }
    } else {
        if (m_file)
            emit errorMessage(tr("Audio format not supported"),
                              formatToString(m_format));
        else
            emit errorMessage(tr("No common input / output format found"), "");
    }

    ENGINE_DEBUG << "FEngine::initialize" << "m_bufferLength" << m_bufferLength;
    ENGINE_DEBUG << "FEngine::initialize" << "m_dataLength" << m_dataLength;
    ENGINE_DEBUG << "FEngine::initialize" << "format" << m_format;

    return result;
}

bool FEngine::selectFormat()
{
    bool foundSupportedFormat = false;

    if (m_file || QAudioFormat() != m_format) {
        QAudioFormat format = m_format;
        if (m_file)
            // Header is read from the WAV file; just need to check whether
            // it is supported by the audio output device
            format = m_file->fileFormat();
        if (m_audioOutputDevice.isFormatSupported(format)) {
            setFormat(format);
            foundSupportedFormat = true;
        }
    } else {

        QList<int> sampleRatesList;
    #ifdef Q_OS_WIN
        // The Windows audio backend does not correctly report format support
        // (see QTBUG-9100).  Furthermore, although the audio subsystem captures
        // at 11025Hz, the resulting audio is corrupted.
        sampleRatesList += 8000;
    #endif

        sampleRatesList += m_audioOutputDevice.supportedSampleRates();
        sampleRatesList = sampleRatesList.toSet().toList(); // remove duplicates
        qSort(sampleRatesList);
        ENGINE_DEBUG << "FEngine::initialize frequenciesList" << sampleRatesList;

        QList<int> channelsList;
        channelsList += m_audioInputDevice.supportedChannelCounts();
        channelsList += m_audioOutputDevice.supportedChannelCounts();
        channelsList = channelsList.toSet().toList();
        qSort(channelsList);
        ENGINE_DEBUG << "FEngine::initialize channelsList" << channelsList;

        QAudioFormat format;
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        int sampleRate, channels;
        foreach (sampleRate, sampleRatesList) {
            if (foundSupportedFormat)
                break;
            format.setSampleRate(sampleRate);
            foreach (channels, channelsList) {
                format.setChannelCount(channels);
                const bool inputSupport = m_audioInputDevice.isFormatSupported(format);
                const bool outputSupport = m_audioOutputDevice.isFormatSupported(format);
                ENGINE_DEBUG << "FEngine::initialize checking " << format
                             << "input" << inputSupport
                             << "output" << outputSupport;
                if (inputSupport && outputSupport) {
                    foundSupportedFormat = true;
                    break;
                }
            }
        }

        if (!foundSupportedFormat)
            format = QAudioFormat();

        setFormat(format);
    }

    return foundSupportedFormat;
}

void FEngine::stopRecording()
{
    if (m_audioInput) {
        m_audioInput->stop();
        QCoreApplication::instance()->processEvents();
        m_audioInput->disconnect();
    }
    m_audioInputIODevice = 0;

#ifdef DUMP_AUDIO
    dumpData();
#endif
}

void FEngine::stopPlayback()
{
    if (m_audioOutput) {
        m_audioOutput->stop();
        QCoreApplication::instance()->processEvents();
        m_audioOutput->disconnect();
        setPlayPosition(0);
    }
}

void FEngine::setState(QAudio::State state)
{
    const bool changed = (m_state != state);
    m_state = state;
    if (changed)
        emit stateChanged(m_mode, m_state);
}

void FEngine::setState(QAudio::Mode mode, QAudio::State state)
{
    const bool changed = (m_mode != mode || m_state != state);
    m_mode = mode;
    m_state = state;
    if (changed)
        emit stateChanged(m_mode, m_state);
}

void FEngine::setRecordPosition(qint64 position, bool forceEmit)
{
    const bool changed = (m_recordPosition != position);
    m_recordPosition = position;
    if (changed || forceEmit)
        emit recordPositionChanged(m_recordPosition);
}

void FEngine::setPlayPosition(qint64 position, bool forceEmit)
{
    const bool changed = (m_playPosition != position);
    m_playPosition = position;
    if (changed || forceEmit)
        emit playPositionChanged(m_playPosition);
}

void FEngine::calculateLevel(qint64 position, qint64 length)
{
#ifdef DISABLE_LEVEL
    Q_UNUSED(position)
    Q_UNUSED(length)
#else
    Q_ASSERT(position + length <= m_bufferPosition + m_dataLength);

    qreal peakLevel = 0.0;

    qreal sum = 0.0;
    const char *ptr = m_buffer.constData() + position - m_bufferPosition;
    const char *const end = ptr + length;
    while (ptr < end) {
        const qint16 value = *reinterpret_cast<const qint16*>(ptr);
        const qreal fracValue = pcmToReal(value);
        peakLevel = qMax(peakLevel, fracValue);
        sum += fracValue * fracValue;
        ptr += 2;
    }
    const int numSamples = length / 2;
    qreal rmsLevel = sqrt(sum / numSamples);

    rmsLevel = qMax(qreal(0.0), rmsLevel);
    rmsLevel = qMin(qreal(1.0), rmsLevel);
    setLevel(rmsLevel, peakLevel, numSamples);

    ENGINE_DEBUG << "FEngine::calculateLevel" << "pos" << position << "len" << length
                 << "rms" << rmsLevel << "peak" << peakLevel;
#endif
}

void FEngine::setFormat(const QAudioFormat &format)
{
    const bool changed = (format != m_format);
    m_format = format;
    m_levelBufferLength = audioLength(m_format, LevelWindowUs);
    if (changed)
        emit formatChanged(m_format);
}

void FEngine::setLevel(qreal rmsLevel, qreal peakLevel, int numSamples)
{
    m_rmsLevel = rmsLevel;
    m_peakLevel = peakLevel;
    emit levelChanged(m_rmsLevel, m_peakLevel, numSamples);
}

#ifdef DUMP_DATA
void FEngine::createOutputDir()
{
    m_outputDir.setPath("output");

    // Ensure output directory exists and is empty
    if (m_outputDir.exists()) {
        const QStringList files = m_outputDir.entryList(QDir::Files);
        QString file;
        foreach (file, files)
            m_outputDir.remove(file);
    } else {
        QDir::current().mkdir("output");
    }
}
#endif // DUMP_DATA

#ifdef DUMP_AUDIO
void FEngine::dumpData()
{
    const QString txtFileName = m_outputDir.filePath("data.txt");
    QFile txtFile(txtFileName);
    txtFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(&txtFile);
    const qint16 *ptr = reinterpret_cast<const qint16*>(m_buffer.constData());
    const int numSamples = m_dataLength / (2 * m_format.channels());
    for (int i=0; i<numSamples; ++i) {
        stream << i << "\t" << *ptr << "\n";
        ptr += m_format.channels();
    }

    const QString pcmFileName = m_outputDir.filePath("data.pcm");
    QFile pcmFile(pcmFileName);
    pcmFile.open(QFile::WriteOnly);
    pcmFile.write(m_buffer.constData(), m_dataLength);
}
#endif // DUMP_AUDIO
