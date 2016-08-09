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

QT_BEGIN_NAMESPACE
class QAudioOutput;
QT_END_NAMESPACE

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
        return CurrentAudioState;
    }

    const QAudioFormat& GetAudioFormat() const
    {
        return CurrentAudioFormat;
    }

    /**
     * Stop any ongoing playback and reset to the ground state.
     */
    void Reset();

signals:
    void AudioFormatChanged(const QAudioFormat& Format);

public slots:
    void Start();
    void Pause();
    void Stop();
    void SetAudioOutputDevice(const QAudioDeviceInfo& Device);

private:
    QAudio::State       CurrentAudioState;
    QAudioFormat        CurrentAudioFormat;

    const QList<QAudioDeviceInfo> AvailableAudioOutputDevices;
    QAudioDeviceInfo    AudioDeviceInfo;
    QAudioOutput*       AudioOutput;
    QIODevice*          AudioOutputIODevice;

    QByteArray          AudioBuffer;
    qint64              AudioBufferLength;
};
