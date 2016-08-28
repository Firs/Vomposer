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
#include "PlaybackModel.h"

#include "Playback.h"
#include "SoundCache.h"

FPlaybackModel::FPlaybackModel(
        FPlayback& Playback,
        FSoundCache& SoundCache,
        QObject* Parent)
    : QObject(Parent)
    , Playback(Playback)
    , SoundCache(SoundCache)
{
}

void FPlaybackModel::playNote(const QString& NoteName)
{
    FSound* Sound = SoundCache.Get(NoteName);
    if (Sound)
    {
        Playback.PlaySound(*Sound);
    }
}

void FPlaybackModel::stop()
{
    Playback.Stop();
}
