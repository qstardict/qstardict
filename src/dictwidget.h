/********************************************************************************
 * dictwidget.h - QStarDict, a StarDict clone written with using Qt             *
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

#ifndef DICTWIDGET_H
#define DICTWIDGET_H

#include <QWidget>
#include "dictcore.h"

class QTextBrowser;

class DictWidget: public QWidget
{
    public:
        DictWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

        void setDict(DictCore *dict);
        const DictCore* dict() const;

        void setTranslationFlags(DictCore::TranslationFlags flags);
        DictCore::TranslationFlags translationFlags() const;

        bool translate(const QString &str);
        QString translatedWord() const;

    private:
        QTextBrowser *translationView;
        DictCore *m_dict;
        DictCore::TranslationFlags m_flags;
        QString m_translatedWord;
};

#endif // DICTWIDGET_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

