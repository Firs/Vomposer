#ifndef FPITCHMONITORVIEWMODEL_H
#define FPITCHMONITORVIEWMODEL_H

#include <QObject>
#include <QString>

class FPitch;
class FPitchMonitor;

class FPitchMonitorModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString PitchName READ GetPitchName NOTIFY PitchChanged)
    Q_PROPERTY(qreal PitchFrequency READ GetPitchFrequency NOTIFY PitchChanged)
    Q_PROPERTY(qreal PrevPitchFrequency READ GetPrevPitchFrequency NOTIFY PitchChanged)
    Q_PROPERTY(qreal NextPitchFrequency READ GetNextPitchFrequency NOTIFY PitchChanged)

    Q_PROPERTY(qreal Frequency READ GetFrequency NOTIFY PitchChanged)

public:
    FPitchMonitorModel(
            FPitchMonitor& Monitor,
            QObject *parent = nullptr);
    virtual ~FPitchMonitorModel();

    QString GetPitchName() const;
    qreal GetPitchFrequency() const;
    qreal GetPrevPitchFrequency() const;
    qreal GetNextPitchFrequency() const;

    qreal GetFrequency() const;

signals:
    void PitchChanged();

private slots:
    void OnPitchUpdated(const FPitch* Pitch, qreal OriginalFrequency);

private:
    FPitchMonitor& PitchMonitor;

    const FPitch* CurrPitch;
    const FPitch* LastPitch;

    qreal Frequency;
};

#endif // FPITCHMONITORVIEWMODEL_H
