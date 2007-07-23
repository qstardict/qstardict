/********************************************************************************
 * selection.cpp                                                                *
 * Copyright (C) 2007 Alexander Rodin                                           *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.        *
 ********************************************************************************/

#include "selection.h"

#include <QApplication>
#include <QClipboard>

Selection::Selection(QObject *parent)
    : QObject(parent)
{
    m_scan = false;
    m_timerId = 0;
}

void Selection::setScan(bool scan)
{
    if (m_scan == scan)
        return;

    m_scan = scan;
    if (m_scan)
    {
        m_lastState = QApplication::clipboard()->text(QClipboard::Selection);
        m_timerId = startTimer(300);
    }
    else
        killTimer(m_timerId);
}

void Selection::timerEvent(QTimerEvent*)
{
    if (m_lastState != QApplication::clipboard()->text(QClipboard::Selection))
    {
        m_lastState = QApplication::clipboard()->text(QClipboard::Selection);
        emit changed(m_lastState);
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

