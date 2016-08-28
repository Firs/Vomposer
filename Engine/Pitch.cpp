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
#include "Pitch.h"

#include <algorithm>

namespace
{

static int MaxId = 0;

// Known pitches within the vocal range, sorted by frequency.
QVector<FPitch> KnownPitches =
{
    { ++MaxId, 'C', false, 2, 65.41  },
    { ++MaxId, 'C', true , 2, 69.30  },
    { ++MaxId, 'D', false, 2, 73.42  },
    { ++MaxId, 'D', true , 2, 77.78  },
    { ++MaxId, 'E', false, 2, 82.41  },
    { ++MaxId, 'F', false, 2, 87.31  },
    { ++MaxId, 'F', true , 2, 92.15  },
    { ++MaxId, 'G', false, 2, 98.00  },
    { ++MaxId, 'G', true , 2, 103.83 },
    { ++MaxId, 'A', false, 2, 110.00 },
    { ++MaxId, 'A', true , 2, 116.54 },
    { ++MaxId, 'B', false, 2, 123.47 },


    { ++MaxId, 'C', false, 3, 130.81 },
    { ++MaxId, 'C', true , 3, 138.59 },
    { ++MaxId, 'D', false, 3, 146.83 },
    { ++MaxId, 'D', true , 3, 155.56 },
    { ++MaxId, 'E', false, 3, 164.81 },
    { ++MaxId, 'F', false, 3, 174.61 },
    { ++MaxId, 'F', true , 3, 185.00 },
    { ++MaxId, 'G', false, 3, 196.00 },
    { ++MaxId, 'G', true , 3, 207.65 },
    { ++MaxId, 'A', false, 3, 220.00 },
    { ++MaxId, 'A', true , 3, 233.08 },
    { ++MaxId, 'B', false, 3, 246.94 },


    { ++MaxId, 'C', false, 4, 261.63 },
    { ++MaxId, 'C', true , 4, 277.18 },
    { ++MaxId, 'D', false, 4, 293.66 },
    { ++MaxId, 'D', true , 4, 311.13 },
    { ++MaxId, 'E', false, 4, 329.63 },
    { ++MaxId, 'F', false, 4, 349.23 },
    { ++MaxId, 'F', true , 4, 369.99 },
    { ++MaxId, 'G', false, 4, 392.00 },
    { ++MaxId, 'G', true , 4, 415.30 },
    { ++MaxId, 'A', false, 4, 440.00 },
    { ++MaxId, 'A', true , 4, 466.16 },
    { ++MaxId, 'B', false, 4, 493.88 },


    { ++MaxId, 'C', false, 5, 523.25 },
    { ++MaxId, 'C', true , 5, 554.37 },
    { ++MaxId, 'D', false, 5, 587.33 },
    { ++MaxId, 'D', true , 5, 622.25 },
    { ++MaxId, 'E', false, 5, 659.25 },
    { ++MaxId, 'F', false, 5, 698.46 },
    { ++MaxId, 'F', true , 5, 739.99 },
    { ++MaxId, 'G', false, 5, 783.99 },
    { ++MaxId, 'G', true , 5, 830.61 },
    { ++MaxId, 'A', false, 5, 880.00 },
    { ++MaxId, 'A', true , 5, 932.33 },
    { ++MaxId, 'B', false, 5, 987.77 },
};

const qreal LowFrequencyDetectionThreshold = 5.0;
const qreal HighFrequencyDetectionThreshold = 50.0;

FPitch* FindClosestPitch(qreal Frequency)
{
    qreal MinDetectableFrequency = KnownPitches[0].Frequency;
    qreal MaxDetectableFrequency = KnownPitches[MaxId - 1].Frequency;

    if ((Frequency < MinDetectableFrequency - LowFrequencyDetectionThreshold) ||
        (Frequency > MaxDetectableFrequency + HighFrequencyDetectionThreshold))
    {
        // Frequency is too low or too high.
        return nullptr;
    }

    // Perform a binary search on the array.
    FPitch SearchValue;
    SearchValue.Frequency = Frequency;
    auto Begin = std::begin(KnownPitches);
    auto End = std::end(KnownPitches);
    auto Found = std::lower_bound(Begin, End, SearchValue);

    if (Found == End)
    {
        return nullptr;
    }

    if (Found != Begin)
    {
        auto PrevPitch = Found - 1;
        qreal DistanceToLower = Frequency - PrevPitch->Frequency;
        qreal DistanceToUpper = Found->Frequency - Frequency;
        if (DistanceToLower < DistanceToUpper)
        {
            // Closest pitch is actually the previous one.
            return PrevPitch;
        }
    }

    return Found;
}

} // namespace

FPitch::FPitch()
{
}

FPitch::FPitch(int Id, char Class, bool bSharp, int Octave, qreal Frequency)
    : Id(Id)
    , Class(Class)
    , bSharp(bSharp)
    , Octave(Octave)
    , Frequency(Frequency)
{
}

const FPitch* FPitch::FromFrequency(qreal Frequency)
{
    return FindClosestPitch(Frequency);
}

bool FPitch::operator <(const FPitch &other) const
{
    return this->Frequency < other.Frequency;
}

const FPitch* FPitch::GetPrevPitch() const
{
    return (Id > 1) ? &KnownPitches[(Id - 1) - 1] : nullptr;
}

const FPitch* FPitch::GetNextPitch() const
{
    return (Id < MaxId) ? &KnownPitches[(Id + 1) - 1] : nullptr;
}


const QVector<FPitch>& FPitch::GetKnownPitches()
{
    return KnownPitches;
}

