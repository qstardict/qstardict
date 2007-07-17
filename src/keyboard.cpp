/********************************************************************************
 * keyboard.cpp - QStarDict, a StarDict clone written with using Qt             *
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

#include "keyboard.h"

#ifdef Q_WS_X11

#include <QX11Info>
#include <X11/XKBlib.h>
#include <stdio.h>

Qt::KeyboardModifiers Keyboard::activeModifiers()
{
    const unsigned mAlt = 010;
    const unsigned mCtrl = 04;
    const unsigned mShift = 01;
    const unsigned mWin = 0100;

    XkbStateRec state;
    Qt::KeyboardModifiers result;

    XkbSelectEvents(QX11Info::display(), XkbUseCoreKbd, XkbStateNotifyMask, XkbStateNotifyMask);
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

#else // Q_WS_X11
Qt::KeyBoardModifiers Keyboard::modifiers()
{
    return Qt::NoModifier;
}
#endif // Q_WS_X11

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

