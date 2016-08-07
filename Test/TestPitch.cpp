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

    void TestPitchMonitorStartup()
    {

            FPitchMonitor Monitor;
            Monitor.Start();

    }
};

QTEST_MAIN(TestPitch)

#include "TestPitch.moc"
