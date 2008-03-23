/*****************************************************************************
 * cssedit.h - QStarDict, a StarDict clone written with using Qt             *
 * Copyright (C) 2008 Alexander Rodin                                        *
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

#ifndef CSSEDIT_H
#define CSSEDIT_H

#include "ui_cssedit.h"

#include <QHash>

namespace QStarDict
{
/**
 * CSS styles editor.
 */
class CSSEdit: public QWidget, private Ui::CSSEdit
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        CSSEdit(QWidget *parent = 0);

        /**
         * Set CSS style for editing.
         */
        void setCSS(const QString &css);
        /**
         * Returns edited CSS style.
         */
        QString css() const;
        /**
         * Set aliases for elements.
         */
        void setElementsAliases(const QHash<QString, QString> &aliases);
        /**
         * Returns elements aliases.
         */
        const QHash<QString, QString> &elementsAliases() const
        { return m_elementsAliases; }

    private slots:
        void colorSelectClicked();
        void setCurrentElement(int index);
        void propertyChanged();

    private:
        typedef QHash<QString, QString> Element;

        void updateElementCombo();
        void updatePreview();
        Element getParentElement(const QString &elementName);

        QHash<QString, Element> m_elements;
        QHash<QString, QString> m_elementsAliases;
        QString m_currentElement;
};
}

#endif // CSSEDIT_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

