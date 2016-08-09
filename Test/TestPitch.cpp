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
#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "Pitch.h"
#include "PitchMonitor.h"

class TestPitch : public QObject
{
    Q_OBJECT

public:
    TestPitch()
    {
    }

private Q_SLOTS:
    void TestPitchFromFrequency()
    {
        const FPitch* Pitch;

        Pitch = FPitch::FromFrequency(442);
        QCOMPARE(Pitch != nullptr, true);
        QCOMPARE(Pitch->Class, 'A');
        QCOMPARE(Pitch->Octave, 4);
        QCOMPARE(Pitch->bSharp, false);
        QCOMPARE(Pitch->Frequency, 440.0);

        Pitch = FPitch::FromFrequency(435);
        QCOMPARE(Pitch != nullptr, true);
        QCOMPARE(Pitch->Class, 'A');
        QCOMPARE(Pitch->Octave, 4);
        QCOMPARE(Pitch->bSharp, false);
        QCOMPARE(Pitch->Frequency, 440.0);

        Pitch = FPitch::FromFrequency(1200.0);
        QCOMPARE(Pitch == nullptr, true);

        Pitch = FPitch::FromFrequency(-500);
        QCOMPARE(Pitch == nullptr, true);
    }
};

QTEST_MAIN(TestPitch)

#include "TestPitch.moc"
