#ifndef FPITCH_H
#define FPITCH_H

#include <QString>

class FPitch
{
public:
    FPitch();
    FPitch(int Id, char Class, bool bSharp, int Octave, qreal Frequency);
    static const FPitch* FromFrequency(qreal Frequency);

    int Id;
    char Class;
    bool bSharp;
    int Octave;
    qreal Frequency;

    inline QString ToString() const
    {
        return QString::asprintf("%c%c%d", Class, (bSharp ? '#' : ' '), Octave);
    }

    bool operator <(const FPitch& other) const;

    const FPitch* GetNextPitch() const;
    const FPitch* GetPrevPitch() const;
};

#endif // FPITCH_H
