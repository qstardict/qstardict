/*****************************************************************************
 * selection.cpp - QStarDict, a StarDict clone written with using Qt         *
 * Copyright (C) 2007 Alexander Rodin                                        *
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

#include "selection.h"

#ifdef Q_WS_WIN
#include <windows.h>

namespace
{

QString currentSelection()
{
    POINT Point;
    HWND hWindow;
    DWORD dwStart, dwEnd;
    char szWindowText[256];

    if (! GetCursorPos(&Point))
        return QString();

    if( ! (hWindow = WindowFromPoint(Point)))
        return QString();

    SendMessage(hWindow, WM_GETTEXT, 256, (LPARAM)szWindowText);
    SendMessage(hWindow, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

    return QString::fromLocal8Bit(szWindowText);
}

}

#else // Q_WS_WIN

#include <QApplication>
#include <QClipboard>

namespace
{

inline QString currentSelection()
{ return QApplication::clipboard()->text(QClipboard::Selection); }

}

#endif // Q_WS_WIN

namespace QStarDict
{

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
        m_lastState = currentSelection();
        m_timerId = startTimer(300);
    }
    else
        killTimer(m_timerId);
}

void Selection::timerEvent(QTimerEvent*)
{
    if (m_lastState != currentSelection())
    {
        m_lastState = currentSelection();
        emit changed(m_lastState);
    }
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

