/*****************************************************************************
 * popupwindow.h - QStarDict, a StarDict clone written with using Qt         *
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

#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "resizablepopup.h"

#include "dictcore.h"
#include "dictwidget.h"

namespace QStarDict
{

class DictWidget;
class Selection;

/**
 * The PopupWindow is a window that scan selection and pop-up
 * when needed.
 */
class PopupWindow: public ResizablePopup
{
    Q_OBJECT

    public:
        /**
         * Construct new PopupWindow.
         */
        PopupWindow(QWidget *parent = 0);
        /**
         * Destructor.
         */
        ~PopupWindow();

        /**
         * Return true if scanning enabled, otherwise return false.
         */
        bool isScan() const;
        /**
         * Return modifier key. The modifiers are from
         * Qt::KeyboardModifier enum.
         */
        int modifierKey() const
        { return m_modifierKey; }
        /**
         * Set behaviour for not found in dictionary words.
         */
        bool showIfNotFound() const
        { return m_showIfNotFound; }

        /**
         * Set the dictionary.
         */
        void setDict(DictCore *dict);

        /**
         * Returns the dictionary.
         */
        DictCore* dict() const
        { return m_dict; }
        /**
         * Return true if shown words will be pronounced, othewise
         * return false.
         */
        bool pronounceWord() const
        { return m_pronounceWord; }

        void setDefaultStyleSheet(const QString &css)
        { translationView->setDefaultStyleSheet(css); }

        QString defaultStyleSheet() const
        { return translationView->defaultStyleSheet(); }

    public slots:
        /**
         * Enable or disable scanning of selection.
         */
        void setScan(bool scan);
        /**
         * Set modifier key. If key not is 0 then popup will be shown
         * only if key is pressed.
         */
        void setModifierKey(int key)
        { m_modifierKey = key; }
        /**
         * Set beahivour for words that not in dictionaris.
         * If true then popup will be shown with text "No found", otherwise
         * popup will not be shown.
         */
        void setShowIfNotFound(bool mode)
        { m_showIfNotFound = mode; }

        /**
         * Popup with translation of text.
         */
        void showTranslation(const QString &text);
        /**
         * If prounounceWord is true shown words will be prounounced.
         */
        void setPronounceWord(bool pronounceWord)
        { m_pronounceWord = pronounceWord; }

        void saveSettings();

    private slots:
        void selectionChanged(const QString &text);

    signals:
        void scanChanged(bool scan);

    private:
        void loadSettings();

        DictWidget *translationView;
        DictCore* m_dict;
        int m_modifierKey;
        bool m_showIfNotFound;
        Selection *m_selection;
    	bool m_pronounceWord;
};

}

#endif // POPUPWINDOW_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

