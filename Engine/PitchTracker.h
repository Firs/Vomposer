#ifndef FPITCHRECOGNIZER_H
#define FPITCHRECOGNIZER_H

#include <QString>

#include "Pitch.h"

class FPitchTracker
{
public:
    FPitchTracker();
    ~FPitchTracker();

    void Update(double* Samples, int StartSample, int SampleCount);

    qreal GetCurrentFrequency() const;

private:
    struct _dywapitchtracker* Tracker;
    qreal CurrentPitchFrequency;
};

#endif // FPITCHRECOGNIZER_H
