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


