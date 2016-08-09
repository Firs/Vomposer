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
