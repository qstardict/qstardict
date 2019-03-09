/*****************************************************************************
 * keyboard.cpp - QStarDict, a quasi-star dictionary                         *
 * Copyright (C) 2007-2019 Alexander Rodin                                   *
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

#include <QApplication>
#include "keyboard.h"

#ifdef Q_OS_WIN

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

#else

namespace QStarDict
{

Qt::KeyboardModifiers Keyboard::activeModifiers()
{
    return QApplication::queryKeyboardModifiers();
}

} // namespace

#endif // Q_OS_WIN

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

