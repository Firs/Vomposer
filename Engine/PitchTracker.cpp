#include "PitchTracker.h"
#include <3rdParty/dywapitchtrack.h>

FPitchTracker::FPitchTracker()
    : Tracker(new dywapitchtracker)
    , CurrentPitchFrequency(0.0)
{
    dywapitch_inittracking(Tracker);
}

FPitchTracker::~FPitchTracker()
{
    delete Tracker;
}

void FPitchTracker::Update(double* Samples, int StartSample, int SampleCount)
{
    CurrentPitchFrequency = dywapitch_computepitch(Tracker, Samples, StartSample, SampleCount);
}

qreal FPitchTracker::GetCurrentFrequency() const
{
    return CurrentPitchFrequency;
}
