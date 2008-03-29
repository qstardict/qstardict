/*****************************************************************************
 * resizablepopup.h - QStarDict, a StarDict clone written with using Qt      *
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

#ifndef RESIZABLEPOPUP_H
#define RESIZABLEPOPUP_H

#include <QFrame>

class QEvent;
class QMouseEvent;
class QTimerEvent;

namespace QStarDict
{

/**
 * The ResizablePopup widget is a resizable top-level window
 * without decorations.
 */
class ResizablePopup: public QFrame
{
    Q_OBJECT

    public:
        /**
         * Construct a ResizablePopup widget.
         */
        ResizablePopup(QWidget *parent = 0);

        /**
         * Return timeout before hiding after mouse leaving.
         */
        int timeoutBeforeHide() const
        { return m_timeoutBeforeHide; }
        /**
         * Return a default size of new-shown popup.
         */
        const QSize& defaultSize() const
        { return m_defaultSize; }

    public slots:
        /**
         * Set timeout before hiding after mouse leaving.
         */
        void setTimeoutBeforeHide(int timeoutBeforeHide)
        { m_timeoutBeforeHide = timeoutBeforeHide; }
        /**
         * Set default size of new-shown popup.
         */
        void setDefaultSize(const QSize &defaultSize)
        { m_defaultSize = defaultSize; }
        /**
         * Show popup under mouse cursor.
         */
        void popup();

    protected:
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
        void mouseMoveEvent(QMouseEvent*);
        void mousePressEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent*);
        void mouseDoubleClickEvent(QMouseEvent*);
        void timerEvent(QTimerEvent*);
        bool event(QEvent *event);

    private:
        void stopResize();
        void doResize();
            
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
        QPoint m_oldCursorPos;
        bool m_isMoving;
        QSize m_defaultSize;
        ResizeDirection m_resizeDirection;
        int m_timeoutBeforeHide;
        int m_timerCloseId;
        int m_timerResizeId;
        bool m_isPopuped;
};

}

#endif // RESIZABLEPOPUP_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

