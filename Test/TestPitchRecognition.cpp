#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <Engine.h>

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
    Engine.startPlayback();

    QThread::sleep(2);
}

QTEST_MAIN(TestPitchRecognition)

#include "TestPitchRecognition.moc"
