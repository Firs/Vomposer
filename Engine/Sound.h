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
#ifndef FSOUND_H
#define FSOUND_H

#include <QBuffer>

struct FSound
{    
    explicit FSound(quint32 BufferSize = 50 * 1024);

    QByteArray  Data;
    bool        bLoops;
    quint32     NumLoops;
    qreal       Volume;
};

#endif // FSOUND_H
