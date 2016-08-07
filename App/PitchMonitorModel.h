#ifndef FPITCHMONITORVIEWMODEL_H
#define FPITCHMONITORVIEWMODEL_H

#include <QObject>
#include <QString>

class FPitch;
class FPitchMonitor;

class FPitchMonitorModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QChar PitchClass READ GetPitchClass NOTIFY pitchChanged)
    Q_PROPERTY(bool PitchSharp READ GetPitchSharp NOTIFY pitchChanged)
    Q_PROPERTY(int PitchOctave READ GetPitchOctave NOTIFY pitchChanged)
    Q_PROPERTY(qreal PitchFrequency READ GetPitchFrequency NOTIFY pitchChanged)
    Q_PROPERTY(qreal PrevPitchFrequency READ GetPrevPitchFrequency NOTIFY pitchChanged)
    Q_PROPERTY(qreal NextPitchFrequency READ GetNextPitchFrequency NOTIFY pitchChanged)

    Q_PROPERTY(qreal Frequency READ GetFrequency NOTIFY pitchChanged)

public:
    FPitchMonitorModel(
            FPitchMonitor& Monitor,
            QObject *parent = nullptr);
    virtual ~FPitchMonitorModel();

    QChar GetPitchClass() const;
    bool GetPitchSharp() const;
    int GetPitchOctave() const;

    qreal GetPitchFrequency() const;
    qreal GetPrevPitchFrequency() const;
    qreal GetNextPitchFrequency() const;

    qreal GetFrequency() const;

signals:
    void pitchChanged();

private slots:
    void OnPitchUpdated(const FPitch* Pitch, qreal OriginalFrequency);

private:
    FPitchMonitor& PitchMonitor;

    const FPitch* CurrPitch;
    const FPitch* LastPitch;

    qreal Frequency;
};

#endif // FPITCHMONITORVIEWMODEL_H
