#pragma once

#include <QtCore/qglobal.h>
#include <QDebug>

QT_FORWARD_DECLARE_CLASS(QAudioFormat)

//-----------------------------------------------------------------------------
// Miscellaneous utility functions
//-----------------------------------------------------------------------------

qint64 CalculateAudioDuration(const QAudioFormat &format, qint64 bytes);
qint64 CalculateAudioLength(const QAudioFormat &format, qint64 microSeconds);

QString AudioFormatToString(const QAudioFormat &format);

// Scale PCM value to [-1.0, 1.0]
qreal PcmToReal(qint16 pcm);

// Scale real value in [-1.0, 1.0] to PCM
qint16 RealToPcm(qreal real);

// Check whether the audio format is signed, little-endian, 16-bit PCM
bool IsPCMS16LE(const QAudioFormat &format);

// Compile-time calculation of powers of two
template<int N> class PowerOfTwo
{ public: static const int Result = PowerOfTwo<N-1>::Result * 2; };

template<> class PowerOfTwo<0>
{ public: static const int Result = 1; };

template <typename T, size_t N>
inline size_t ArraySize(const T(&)[N])
{
    return N;
}

// Macro which connects a signal to a slot, and which causes application to
// abort if the connection fails.  This is intended to catch programming errors
// such as mis-typing a signal or slot name.  It is necessary to write our own
// macro to do this - the following idiom
//     Q_ASSERT(connect(source, signal, receiver, slot));
// will not work because Q_ASSERT compiles to a no-op in release builds.

#define CHECKED_CONNECT(source, signal, receiver, slot) \
    if (!connect(source, signal, receiver, slot)) \
        qt_assert_x(Q_FUNC_INFO, "CHECKED_CONNECT failed", __FILE__, __LINE__);

