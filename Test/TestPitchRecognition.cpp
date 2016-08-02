#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "Engine.h"
#include "Pitch.h"

class TestPitchRecognition : public QObject
{
    Q_OBJECT

public:
    TestPitchRecognition();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void TestCase1_data();
    void TestCase1();
    void TestPitchFromFrequency();
};

TestPitchRecognition::TestPitchRecognition()
{
}

void TestPitchRecognition::initTestCase()
{
}

void TestPitchRecognition::cleanupTestCase()
{
}

void TestPitchRecognition::TestCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void TestPitchRecognition::TestCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");

    FEngine Engine;
    Engine.loadFile("Data/Test/C3.wav");
}

void TestPitchRecognition::TestPitchFromFrequency()
{
    {
        auto Pitch = FPitch::FromFrequency(442);
        QCOMPARE(Pitch != nullptr, true);
        QCOMPARE(Pitch->Class, 'A');
        QCOMPARE(Pitch->Octave, 4);
        QCOMPARE(Pitch->bSharp, false);
        QCOMPARE(Pitch->Frequency, 440.0);
    }
    {
        auto Pitch = FPitch::FromFrequency(435);
        QCOMPARE(Pitch != nullptr, true);
        QCOMPARE(Pitch->Class, 'A');
        QCOMPARE(Pitch->Octave, 4);
        QCOMPARE(Pitch->bSharp, false);
        QCOMPARE(Pitch->Frequency, 440.0);
    }
    {
        auto Pitch = FPitch::FromFrequency(1100);
        QCOMPARE(Pitch == nullptr, true);
    }
    {
        auto Pitch = FPitch::FromFrequency(-500);
        QCOMPARE(Pitch == nullptr, true);
    }
}

QTEST_MAIN(TestPitchRecognition)

#include "TestPitchRecognition.moc"
