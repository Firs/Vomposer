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
#include "Synthesizer.h"

#include <QAudioFormat>
#include <QByteArray>
#include <qmath.h>
#include <qendian.h>

#include "Pitch.h"
#include "Utils.h"

FSynthesizer::FSynthesizer()
{

}

void FSynthesizer::GenerateSineWave(
        const FPitch& Pitch, const QAudioFormat& Format, QByteArray& Buffer)
{
    Q_ASSERT(IsPCMS16LE(Format));
    Q_ASSERT(Pitch.Frequency > 0);

    const int channelBytes = Format.sampleSize() / 8;
    const int sampleBytes = Format.channelCount() * channelBytes;
    int length = Buffer.size();
    const int numSamples = Buffer.size() / sampleBytes;

    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds


    quint8* ptr = reinterpret_cast<quint8*>(Buffer.data());

    qreal Phase = 0.0;

    // We can't generate a zero-frequency sine wave
    const qreal Freq = Pitch.Frequency > 0 ? Pitch.Frequency : 1.0;

    // Amount by which phase increases on each sample
    const qreal PhaseStep = 2 * M_PI / Format.sampleRate() * Freq;

    while (length) {
        const qreal x = qSin(Phase);
        const qint16 value = RealToPcm(x);

        for (int ChannelIdx = 0; ChannelIdx < Format.channelCount(); ++ChannelIdx)
        {
            qToLittleEndian<qint16>(value, ptr);
            ptr += channelBytes;
            length -= channelBytes;
        }

        Phase += PhaseStep;
        while (Phase > 2 * M_PI)
        {
            Phase -= 2 * M_PI;
        }
    }
}
