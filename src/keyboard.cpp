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

