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
#include <QScopedPointer>
#include <QAudioOutput>

#include "Api.h"


struct FSound;

/**
 * This class interfaces with the Qt Multimedia audio classes.
 * Its role is to provide playback functionality.
 */
class API_EXPORT FPlayback : public QObject
{
    Q_OBJECT
public:
    explicit FPlayback(QObject *parent = nullptr);

    const QList<QAudioDeviceInfo>& GetAvailableAudioOutputDevices() const
    {
        return AvailableAudioOutputDevices;
    }

    QAudio::State GetAudioState() const
    {
        return AudioState;
    }

    const QAudioFormat& GetAudioFormat() const
    {
        return AudioFormat;
    }

    /**
     * Stop any ongoing playback and reset to the ground state.
     */
    void Reset();

    void PlaySound(const FSound& Sound);

public slots:
    void Stop();
    void Start();
    void Pause();

    void SetAudioOutputDevice(const QAudioDeviceInfo& Device);

signals:
    void AudioFormatChanged(const QAudioFormat& Format);

private slots:
    void OnAudioStateChanged(QAudio::State State);
    void OnAudioNotify();

private:
    bool Initialize();
    void ResetAudioDevice();
    bool SelectAudioFormat();

    void SetAudioFormat(const QAudioFormat& format);

private:
    QAudio::State       AudioState;
    QAudioFormat        AudioFormat;

    const QList<QAudioDeviceInfo>   AvailableAudioOutputDevices;
    QAudioDeviceInfo                AudioDeviceInfo;
    QScopedPointer<QAudioOutput>    AudioOutput;
    QBuffer                         AudioOutputBuffer;
};
