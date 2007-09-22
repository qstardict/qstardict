/*****************************************************************************
 * dictwidget.h - QStarDict, a StarDict clone written with using Qt          *
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

#ifndef DICTWIDGET_H
#define DICTWIDGET_H

#include <QFrame>
#include "dictcore.h"

class QTextBrowser;

class DictWidget: public QFrame
{
    public:
        DictWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

        void setDict(DictCore *dict)
        { m_dict = dict; }
        const DictCore* dict() const
        { return m_dict; }
        void clear();

        void setTranslationFlags(DictCore::TranslationFlags flags)
        { m_translationFlags = flags; }
        DictCore::TranslationFlags translationFlags() const
        { return m_translationFlags; }

        bool translate(const QString &str);
        QString translatedWord() const;

        static const QString &cssStyle();

    private:
        QTextBrowser *translationView;
        DictCore *m_dict;
        DictCore::TranslationFlags m_translationFlags;
        QString m_translatedWord;
};

#endif // DICTWIDGET_H
