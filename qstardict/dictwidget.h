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

class DictBrowser;

/**
 * The DictBrowser widget provides view of translations from given dictionary.
 */
class DictWidget: public QFrame
{
    Q_OBJECT

    public:
        /**
         * Construct empty DictWidget.
         */
        DictWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

        /**
         * Set source dictionary.
         * Warning: DictWidget copy only a pointer to dict. So set dictionaries
         * allocated from heap and don't destroy it befor DictWidget.
         */
        void setDict(DictCore *dict)
        { m_dict = dict; }
        /**
         * Return pointer to dictionary.
         */
        const DictCore* dict() const
        { return m_dict; }
        /**
         * Clear translation text.
         */
        void clear();

        /**
         * Set translation flags.
         */
        void setTranslationFlags(DictCore::TranslationFlags flags)
        { m_translationFlags = flags; }
        /**
         * Return translation flags.
         */
        DictCore::TranslationFlags translationFlags() const
        { return m_translationFlags; }

        /**
         * Show translation of str. If no translations found return false, 
         * otherwise true.
         */
        bool translate(const QString &str);
        /**
         * Return last translated word.
         */
        QString translatedWord() const;

        /**
         * Return CSS style used by DictWidget. It can be used to generate
         * HTML translation with like DictWidget apperance.
         */
        static QString cssStyle();

    signals:
        /**
         * Emits when translated word is shown.
         */
        void wordTranslated(const QString &word);

    private:
        DictBrowser *translationView;
        DictCore *m_dict;
        DictCore::TranslationFlags m_translationFlags;
        QString m_translatedWord;
};

#endif // DICTWIDGET_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

