#ifndef FNOTE_H
#define FNOTE_H

#include "Pitch.h"

struct FNote
{
public:
    FPitch Pitch;
    qreal Duration;
    qreal ImpactVelocity;
    qreal PitchBend;
};

#endif // FNOTE_H
