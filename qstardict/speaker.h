/*****************************************************************************
 * speaker.h - QStarDict, a StarDict clone written with using Qt             *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#ifndef SPEAKER_H
#define SPEAKER_H

#include <QString>

class QProcess;

namespace QStarDict
{

class Speaker
{
    public:
        Speaker();
        virtual ~Speaker();

        void speak(const QString &word);

        void setSpeechCmd(const QString &cmd)
        { m_speechCmd = cmd; }
        const QString &speechCmd() const
        { return m_speechCmd; }

    private:
        QString m_speechCmd;
        QProcess *m_speechProcess;
};

}

#endif // SPEAKER_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

