#include "Pitch.h"
#include <algorithm>
#include "Platform/Utils.h"

namespace
{

// Known pitches within the vocal range, sorted by frequency.
FPitch KnownPitches[] =
{
    { 'C', false, 2, 65.41  },
    { 'C', true , 2, 69.30  },
    { 'D', false, 2, 73.42  },
    { 'D', true , 2, 77.78  },
    { 'E', false, 2, 82.41  },
    { 'F', false, 2, 87.31  },
    { 'F', true , 2, 92.15  },
    { 'G', false, 2, 98.00  },
    { 'G', true , 2, 103.83 },
    { 'A', false, 2, 110.00 },
    { 'A', true , 2, 116.54 },
    { 'B', false, 2, 123.47 },


    { 'C', false, 3, 130.81 },
    { 'C', true , 3, 138.59 },
    { 'D', false, 3, 146.83 },
    { 'D', true , 3, 155.56 },
    { 'E', false, 3, 164.81 },
    { 'F', false, 3, 174.61 },
    { 'F', true , 3, 185.00 },
    { 'G', false, 3, 196.00 },
    { 'G', true , 3, 207.65 },
    { 'A', false, 3, 220.00 },
    { 'A', true , 3, 233.08 },
    { 'B', false, 3, 246.94 },


    { 'C', false, 4, 261.63 },
    { 'C', true , 4, 277.18 },
    { 'D', false, 4, 293.66 },
    { 'D', true , 4, 311.13 },
    { 'E', false, 4, 329.63 },
    { 'F', false, 4, 349.23 },
    { 'F', true , 4, 369.99 },
    { 'G', false, 4, 392.00 },
    { 'G', true , 4, 415.30 },
    { 'A', false, 4, 440.00 },
    { 'A', true , 4, 466.16 },
    { 'B', false, 4, 493.88 },


    { 'C', false, 5, 523.25 },
    { 'C', true , 5, 554.37 },
    { 'D', false, 5, 587.33 },
    { 'D', true , 5, 622.25 },
    { 'E', false, 5, 659.25 },
    { 'F', false, 5, 698.46 },
    { 'F', true , 5, 739.99 },
    { 'G', false, 5, 783.99 },
    { 'G', true , 5, 830.61 },
    { 'A', false, 5, 880.00 },
    { 'A', true , 5, 932.33 },
    { 'B', false, 5, 987.77 },
};

const qreal LowFrequencyDetectionThreshold = 5.0;
const qreal HighFrequencyDetectionThreshold = 50.0;

FPitch* FindClosestPitch(qreal Frequency)
{
    size_t Size = SizeOfArray(KnownPitches);
    qreal MinDetectableFrequency = KnownPitches[0].Frequency;
    qreal MaxDetectableFrequency = KnownPitches[Size - 1].Frequency;

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
    auto Iter = std::lower_bound(Begin, End, SearchValue);

    if (Iter == End)
    {
        // Should not happen.
        return nullptr;
    }

    if (Iter != Begin)
    {
        auto PrevPitch = Iter - 1;
        qreal Delta1 = Frequency - PrevPitch->Frequency;
        qreal Delta2 = Iter->Frequency - Frequency;
        if (Delta1 < Delta2)
        {
            // Closest pitch is actually the previous one.
            return PrevPitch;
        }
    }

    return Iter;
}

}

FPitch::FPitch()
{
}

FPitch::FPitch(char Class, bool bSharp, int Octave, qreal Frequency)
    : Class(Class)
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


