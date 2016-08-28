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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QDebug>
#include <QElapsedTimer>

#include "PitchMonitor.h"
#include "PitchMonitorModel.h"

#include "SoundCache.h"
#include "Playback.h"
#include "PlaybackModel.h"

#include "Synthesizer.h"

void GenerateSounds(const QAudioFormat& Format, FSoundCache& Cache)
{

    qDebug() << "Building sound cache...";

    QElapsedTimer Timer;
    Timer.start();


    FSynthesizer Synth;

    Cache.Sounds.reserve(FPitch::GetKnownPitches().size());

    for(const auto& Pitch : FPitch::GetKnownPitches())
    {
        FSound* Sound = Cache.Add(Pitch.ToString());
        if (Sound)
        {
            Synth.GenerateSineWave(Pitch, Format, Sound->Data);
        }
    }

    qDebug() << "Initialized"  << Cache.GetSize() << "pitches in" << Timer.elapsed() << "ms";

}

int main(int argc, char *argv[])
{
    QGuiApplication App(argc, argv);
    QQmlApplicationEngine AppEngine;

    FPitchMonitor Monitor;
    FPitchMonitorModel MonitorModel(Monitor);
    AppEngine.rootContext()->setContextProperty(
                "PitchMonitor", static_cast<QObject*>(&MonitorModel));

    FPlayback Playback;
    FSoundCache SoundCache;
    FPlaybackModel PlaybackModel(Playback, SoundCache);
    AppEngine.rootContext()->setContextProperty(
                "Playback", static_cast<QObject*>(&PlaybackModel));

    AppEngine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    GenerateSounds(Playback.GetAudioFormat(), SoundCache);

    //PlaybackModel.PlayNote("A4");

    return App.exec();
}
