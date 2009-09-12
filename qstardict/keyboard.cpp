/*****************************************************************************
 * keyboard.cpp - QStarDict, a StarDict clone written with using Qt          *
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

#include "keyboard.h"

#ifdef Q_WS_X11

#include <QX11Info>
#include <X11/XKBlib.h>
#include <stdio.h>

namespace
{
const unsigned mAlt     = 0010;
const unsigned mCtrl    = 0004;
const unsigned mShift   = 0001;
const unsigned mWin     = 0100;
}

namespace QStarDict 
{

Qt::KeyboardModifiers Keyboard::activeModifiers()
{
    XkbStateRec state;
    Qt::KeyboardModifiers result;

    XkbGetState(QX11Info::display(), XkbUseCoreKbd, &state);
    if (state.base_mods & mAlt)
        result |= Qt::AltModifier;
    if (state.base_mods & mCtrl)
        result |= Qt::ControlModifier;
    if (state.base_mods & mShift)
        result |= Qt::ShiftModifier;
    if (state.base_mods & mWin)
        result |= Qt::MetaModifier;

    return result;
}

} // namespace

#elif defined(Q_WS_WIN) // Q_WS_X11

#include <windows.h>
#include <winuser.h>

namespace QStarDict
{

Qt::KeyboardModifiers Keyboard::activeModifiers()
{
    Qt::KeyboardModifiers result;

    if (GetAsyncKeyState(VK_MENU) & 0x8000)
        result |= Qt::AltModifier;
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
        result |= Qt::ControlModifier;
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
        result |= Qt::ShiftModifier;
    if ((GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000))
        result |= Qt::MetaModifier;

    return result;
}

} // namespace

#elif defined(Q_WS_MAC) // Q_WS_WIN
#include <QApplication>

namespace QStarDict
{

Qt::KeyboardModifiers Keyboard::activeModifiers()
{
    return QApplication::keyboardModifiers();
}

} // namespace
#endif // Q_WS_MAC

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

