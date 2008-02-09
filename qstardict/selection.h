/*****************************************************************************
 * selection.h - QStarDict, a StarDict clone written with using Qt           *
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

#ifndef SELECTION_H
#define SELECTION_H

#include <QObject>

class QTimerEvent;

namespace QStarDict
{

/**
 * Mouse selection representation class.
 */
class Selection: public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        Selection(QObject *parent = 0);

        /**
         * Return scan state.
         */
        bool isScan() const
        { return m_scan; }

    public slots:
        /**
         * Set scan mode.
         */
        void setScan(bool scan);

    signals:
        /**
         * Emits when selection text is changed.
         */
        void changed(const QString &newText);

    protected:
        void timerEvent(QTimerEvent*);

    private:
        QString m_lastState;
        int m_scan;
        int m_timerId;
};

}

#endif // SELECTION_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

