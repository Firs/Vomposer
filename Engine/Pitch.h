#ifndef FPITCH_H
#define FPITCH_H

#include <QString>

struct FPitch
{
public:
    FPitch();
    FPitch(char Class, bool bSharp, quint8 Octave, qreal Frequency);
    static const FPitch* FromFrequency(qreal Frequency);

    char Class;
    bool bSharp;
    quint8 Octave;
    qreal Frequency;

    inline QString ToString() const
    {
        return QString::asprintf("%c%c%d", Class, (bSharp ? '#' : ' '), Octave);
    }
};

#endif // FPITCH_H
