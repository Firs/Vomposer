#ifndef PITCHMONITOR_H
#define PITCHMONITOR_H

#include <Global.h>

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QBuffer>
#include <QByteArray>
#include <QObject>
#include <QVector>
#include <vector>

#include "PitchTracker.h"

QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE

/**
 * This class interfaces with the Qt Multimedia audio classes.
 * Its role is to capture and extract pitch from audio data.
 */
class ENGINESHARED_EXPORT FPitchMonitor : public QObject
{
    Q_OBJECT

public:
    explicit FPitchMonitor(QObject *parent = 0);
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
    void PitchDetected(const FPitch* Pitch);
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

    std::vector<qreal>  RecordedSamples;
    FPitchTracker       PitchTracker;
};

#endif
