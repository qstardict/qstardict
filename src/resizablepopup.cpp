/********************************************************************************
 * resizablepopup.cpp - QStarDict, a StarDict clone written with using Qt       *
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

#include "resizablepopup.h"

#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QTimerEvent>

namespace
{
const int CornerSize = 10;
}

ResizablePopup::ResizablePopup(QWidget *parent)
    : QFrame(parent, Qt::Popup)
{
    m_resizeDirection = None;
    m_timeoutBeforeHide = 0;
    m_timerCloseId = 0;
    m_timerResizeId = 0;
    setMouseTracking(true);
    setLineWidth(2);
    setMidLineWidth(1);
    setFrameStyle(QFrame::Box);
    setFrameShadow(QFrame::Raised);
}

void ResizablePopup::popup()
{
    if (m_defaultSize != size())
        resize(m_defaultSize);
    QPoint newPosition = QCursor::pos() - QPoint(30, 30);
    if (newPosition.x() < 0)
        newPosition.setX(0);
    else if (newPosition.x() + width() > QApplication::desktop()->width())
        newPosition.setX(QApplication::desktop()->width() - width());
    if (newPosition.y() < 0)
        newPosition.setY(0);
    else if (newPosition.y() + height() > QApplication::desktop()->height())
        newPosition.setY(QApplication::desktop()->height() - height());
    move(newPosition);
    show();
}

void ResizablePopup::enterEvent(QEvent*)
{
    if (m_timerCloseId)
    {
        killTimer(m_timerCloseId);
        m_timerCloseId = 0;
    }
}

void ResizablePopup::leaveEvent(QEvent*)
{
    if (m_timeoutBeforeHide < 0)
        return;
    else if (m_timeoutBeforeHide == 0)
        hide();
    else if (! m_timerCloseId)
        m_timerCloseId = startTimer(m_timeoutBeforeHide);
}

void ResizablePopup::mouseMoveEvent(QMouseEvent *event)
{
    const int CornerSize = 10;

    Qt::CursorShape cursorShape = Qt::ArrowCursor;
    if ((event->x() >= 0 && event->x() < CornerSize &&
            event->y() >= 0 && event->y() < CornerSize) ||
        (event->x() < width() && event->x() >= width() - CornerSize &&
            event->y() < height() && event->y() >= height() - CornerSize))
        cursorShape = Qt::SizeFDiagCursor;
    else if ((event->x() < width() && event->x() >= width() - CornerSize &&
                event->y() >= 0 && event->y() < CornerSize) ||
             (event->x() >= 0 && event->x() < CornerSize &&
                event->y() < height() && event->y() >= height() - CornerSize))
        cursorShape = Qt::SizeBDiagCursor;
    else if (event->x() >= 0 && event->x() < frameWidth() ||
             event->x() < width() && event->x() >= width() - frameWidth())
        cursorShape = Qt::SizeHorCursor;
    else if (event->y() >= 0 && event->y() < frameWidth() ||
             event->y() < height() && event->y() >= height() - frameWidth())
        cursorShape = Qt::SizeVerCursor;
    
    if (cursor().shape() != cursorShape)
        setCursor(cursorShape);
}

void ResizablePopup::mousePressEvent(QMouseEvent *event)
{
    if (! geometry().contains(event->globalPos()))
    {
        if (m_timerCloseId)
        {
            killTimer(m_timerCloseId);
            m_timerCloseId = 0;
        }
        hide();
        return;
    }

    if (event->buttons().testFlag(Qt::LeftButton))
    {
        if (event->x() < CornerSize && event->y() < CornerSize)
            m_resizeDirection = TopLeft;
        else if (event->x() >= width() - CornerSize && event->y() < CornerSize)
            m_resizeDirection = TopRight;
        else if (event->x() < CornerSize && event->y() >= height() - CornerSize)
            m_resizeDirection = BottomLeft;
        else if (event->x() >= width() - CornerSize && event->y() >= height() - CornerSize)
            m_resizeDirection = BottomRight;
        else if (event->x() < frameWidth())
            m_resizeDirection = Left;
        else if (event->x() >= width() - frameWidth())
            m_resizeDirection = Right;
        else if (event->y() < frameWidth())
            m_resizeDirection = Top;
        else if (event->y() >= height() - frameWidth())
            m_resizeDirection = Bottom;
        else
            m_resizeDirection = None;
        if (m_resizeDirection)
            m_timerResizeId = startTimer(8);
    }
}

void ResizablePopup::mouseReleaseEvent(QMouseEvent*)
{
    stopResize();
}

void ResizablePopup::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerResizeId)
    {
        doResize();
    }
    else if (event->timerId() == m_timerCloseId)
    {
        hide();
        killTimer(m_timerCloseId);
        m_timerCloseId = 0;
        stopResize();
    }
}

void ResizablePopup::doResize()
{
    if (! QApplication::mouseButtons().testFlag(Qt::LeftButton))
        stopResize();
    
    if (m_resizeDirection)
    {
        QRect newGeometry = geometry();
        switch (m_resizeDirection)
        {
            case TopLeft:
                newGeometry.setTopLeft(QCursor::pos());
                break;
            case TopRight:
                newGeometry.setTopRight(QCursor::pos());
                break;
            case BottomLeft:
                newGeometry.setBottomLeft(QCursor::pos());
                break;
            case BottomRight:
                newGeometry.setBottomRight(QCursor::pos());
                break;
            case Left:
                newGeometry.setLeft(QCursor::pos().x());
                break;
            case Right:
                newGeometry.setRight(QCursor::pos().x());
                break;
            case Top:
                newGeometry.setTop(QCursor::pos().y());
                break;
            case Bottom:
                newGeometry.setBottom(QCursor::pos().y());
                break;
            default:
                ; // Nothing
        }
        if (newGeometry.width() < minimumSize().width())
        {
            newGeometry.setWidth(width());
            newGeometry.moveLeft(geometry().left());
        }
        if (newGeometry.height() < minimumSize().height())
        {
            newGeometry.setHeight(height());
            newGeometry.moveTop(geometry().top());
        }
        if (newGeometry != geometry())
            setGeometry(newGeometry);
    }
}

void ResizablePopup::stopResize()
{
    if (m_resizeDirection)
    {
        m_resizeDirection = None;
        killTimer(m_timerResizeId);
        m_timerResizeId = 0;
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

