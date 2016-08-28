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
#ifndef FPLAYBACKMODEL_H
#define FPLAYBACKMODEL_H

#include <QObject>
#include <QString>

class FPlayback;
class FSoundCache;

class FPlaybackModel : public QObject
{
    Q_OBJECT

public:
    FPlaybackModel(
            FPlayback& Playback,
            FSoundCache& SoundCache,
            QObject* Parent = nullptr);

    Q_INVOKABLE void playNote(const QString& NoteName);
    Q_INVOKABLE void stop();

signals:

public slots:

private:
    FPlayback& Playback;
    FSoundCache& SoundCache;
};

#endif // FPLAYBACKMODEL_H
