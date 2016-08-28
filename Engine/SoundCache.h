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
#ifndef FSOUNDCACHE_H
#define FSOUNDCACHE_H

#include "Sound.h"

#include <QMap>
#include <QString>
#include <QVector>

class FSoundCache
{
public:
    FSoundCache();

    FSound* Add(const QString& SoundName);
    FSound* Get(const QString& SoundName) const;
    quint32 GetSize() const
    {
        return SoundMap.size();
    }


    QMap<QString, FSound*> SoundMap;
    QVector<FSound> Sounds;

};

#endif // FSOUNDCACHE_H
