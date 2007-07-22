/********************************************************************************
 * resizablepopup.h - QStarDict, a StarDict clone written with using Qt         *
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

#ifndef RESIZABLEPOPUP_H
#define RESIZABLEPOPUP_H

#include <QFrame>

class QEvent;
class QMouseEvent;
class QTimerEvent;

class ResizablePopup: public QFrame
{
    Q_OBJECT

    public:
        ResizablePopup(QWidget *parent = 0);

        int timeoutBeforeHide() const;
        const QSize& defaultSize() const;

    public slots:
        void setTimeoutBeforeHide(int timeoutBeforeHide);
        void setDefaultSize(const QSize &defaultSize);
        void popup();

    protected:
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
        void mouseMoveEvent(QMouseEvent*);
        void mousePressEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent);
        void timerEvent(QTimerEvent*);

    private:
        enum ResizeDirection
        {
            None,
            Top,
            Bottom,
            Left,
            Right,
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight
        };
        QSize m_defaultSize;
        ResizeDirection m_resizeDirection;
        int m_timeoutBeforeHide;
        int m_timerCloseId;
};

#endif // RESIZABLEPOPUP_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

