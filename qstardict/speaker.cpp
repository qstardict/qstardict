/*****************************************************************************
 * speaker.cpp - QStarDict, a StarDict clone written using Qt                *
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

#include "speaker.h"

#include <QProcess>
#include <QSettings>

namespace QStarDict
{

Speaker::Speaker()
{
    m_speechProcess = new QProcess;
    QSettings settings;
    m_speechCmd = settings.value("Speaker/speechCmd", "festival --tts").toString();
}

Speaker::~Speaker()
{
    QSettings settings;
    settings.setValue("Speaker/speechCmd", m_speechCmd);
    delete m_speechProcess;
}

void Speaker::speak(const QString &word)
{
    if (m_speechCmd.isEmpty())
        return;

    if (m_speechProcess->state() != QProcess::NotRunning)
    m_speechProcess->kill();
    
    QString s = m_speechCmd;
    s.replace("%s", word);
    m_speechProcess->start(s, QIODevice::WriteOnly);
    if (! m_speechProcess->waitForStarted())
        return;
    if (! m_speechCmd.contains("%s"))
    {
        m_speechProcess->write(word.toUtf8());
        m_speechProcess->closeWriteChannel();
    }
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

