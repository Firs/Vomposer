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
#pragma once

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QBuffer>
#include <QByteArray>
#include <QObject>
#include <QVector>

#include "Api.h"
#include "PitchTracker.h"

QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE

/**
 * This class interfaces with the Qt Multimedia audio classes.
 * Its role is to capture and extract pitch from audio data.
 */
class API_EXPORT FPitchMonitor : public QObject
{
    Q_OBJECT

public:
    explicit FPitchMonitor(QObject* parent = nullptr);
    ~FPitchMonitor();

    const QList<QAudioDeviceInfo>& GetAvailableAudioInputDevices() const
    {
        return AvailableAudioInputDevices;
    }

    QAudio::State GetAudioState() const
    {
        return CurrentAudioState;
    }

    const QAudioFormat& GetAudioFormat() const
    {
        return CurrentAudioFormat;
    }

    /**
     * Stop any ongoing monitoring and reset to ground state.
     */
    void Reset();

public slots:
    void Start();
    void Pause();
    void Stop();

    void SetAudioInputDevice(const QAudioDeviceInfo &device);

signals:
    void PitchDetected(const FPitch* Pitch, qreal OriginalFrequency);
    void AudioFormatChanged(const QAudioFormat& Format);

private slots:
    void OnAudioDataReady();
    void OnAudioStateChanged(QAudio::State State);

private:
    void ResetAudioDevices();
    bool Initialize();
    bool SelectAudioFormat();

    void SetAudioState(QAudio::State State);
    void SetAudioFormat(const QAudioFormat& format);

private:
    QAudio::State       CurrentAudioState;
    QAudioFormat        CurrentAudioFormat;

    const QList<QAudioDeviceInfo> AvailableAudioInputDevices;
    QAudioDeviceInfo    AudioInputDevice;
    QAudioInput*        AudioInput;
    QIODevice*          AudioInputIODevice;

    QByteArray          AudioBuffer;
    qint64              AudioBufferLength;

    QVector<qreal>      RecordedSamples;
    FPitchTracker       PitchTracker;
};

