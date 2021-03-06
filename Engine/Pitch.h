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
#pragma once

#include <QString>
#include <QVector>

#include "Api.h"

class API_EXPORT FPitch
{
public:
    FPitch();
    FPitch(int Id, char Class, bool bSharp, int Octave, qreal Frequency);

    static const FPitch* FromFrequency(qreal Frequency);
    static const QVector<FPitch>& GetKnownPitches();

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


