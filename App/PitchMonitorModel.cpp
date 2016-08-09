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
#include "PitchMonitorModel.h"

#include "PitchMonitor.h"
#include "Utils.h"

FPitchMonitorModel::FPitchMonitorModel(
        FPitchMonitor &Monitor, QObject *Parent)
    : QObject(Parent)
    , PitchMonitor(Monitor)
    , CurrPitch(nullptr)
    , LastPitch(nullptr)
    , Frequency(0.0)
{
    CHECKED_CONNECT(&PitchMonitor, SIGNAL(PitchDetected(const FPitch*, qreal)),
                    this, SLOT(OnPitchUpdated(const FPitch*, qreal)));
    PitchMonitor.Start();
}

FPitchMonitorModel::~FPitchMonitorModel()
{
}

QChar FPitchMonitorModel::GetPitchClass() const
{
    return CurrPitch ? CurrPitch->Class : 0;
}

bool FPitchMonitorModel::GetPitchSharp() const
{
    return CurrPitch ? CurrPitch->bSharp : false;
}

int FPitchMonitorModel::GetPitchOctave() const
{
    return CurrPitch ? CurrPitch->Octave : 0;
}

qreal FPitchMonitorModel::GetPitchFrequency() const
{
    return CurrPitch ? CurrPitch->Frequency : 0.0;
}

qreal FPitchMonitorModel::GetPrevPitchFrequency() const
{
    const FPitch* PrevPitch = CurrPitch ? CurrPitch->GetPrevPitch(): nullptr;
    return PrevPitch ? PrevPitch->Frequency : 0.0;
}

qreal FPitchMonitorModel::GetNextPitchFrequency() const
{
    const FPitch* NextPitch = CurrPitch ? CurrPitch->GetNextPitch() : nullptr;
    return NextPitch ? NextPitch->Frequency : 0.0;
}

qreal FPitchMonitorModel::GetFrequency() const
{
    return Frequency;
}


void FPitchMonitorModel::OnPitchUpdated(const FPitch* Pitch, qreal OriginalFrequency)
{
    if (LastPitch == nullptr)
    {
        // Filter out lonely pitches and random noise.
        LastPitch = Pitch;
        return;
    }

    LastPitch = Pitch;

    CurrPitch = Pitch;
    Frequency = OriginalFrequency;

    emit pitchChanged();
}


