/*****************************************************************************
 * dictbrowser.h - QStarDict, a StarDict clone written with using Qt         *
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

#ifndef DICTBROWSER_H
#define DICTBROWSER_H

#include <QTextBrowser>

#include <QTextCursor>
#include <QTextCharFormat>
#include "dictcore.h"

namespace QStarDict
{

/**
 * The DictBrowser widget provides view of translations from given dictionary.
 */
class DictBrowser: public QTextBrowser
{
    Q_OBJECT
    public:
        /**
         * Construct empty DictWidget.
         */
        DictBrowser(QWidget *parent = 0);

        /**
         * Set source dictionary.
         * Warning: DictBrowser will copy only a pointer to dict. So set dictionaries
         * allocated from heap and don't destroy it befor DictWidget.
         */
        void setDict(DictCore *dict)
        { m_dict = dict; }
        /**
         * Return pointer to dictionary.
         */
        const DictCore* dict() const
        { return m_dict; }

        QVariant loadResource(int type, const QUrl &name);

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private slots:
        void on_anchorClicked(const QUrl &link);

    private:
        DictCore *m_dict;

        QTextCursor m_oldCursor;
        QTextCharFormat m_oldFormat;
        bool m_highlighted;
};

}

#endif // DICTBROWSER_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

