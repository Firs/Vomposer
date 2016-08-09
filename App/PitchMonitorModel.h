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
