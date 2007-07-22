/********************************************************************************
 * popupwindow.h - QStarDict, a StarDict clone written with using Qt            *
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

#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "resizablepopup.h"
#include "dictcore.h"

class DictWidget;
class Selection;

class PopupWindow: public ResizablePopup
{
    Q_OBJECT

    public:
        PopupWindow(DictCore *dict = 0, QWidget *parent = 0);
        ~PopupWindow();

        bool isScan() const;
        int modifierKey() const;
        bool showIfNotFound() const;
        DictCore::TranslationFlags translationFlags() const;

        DictCore* dict() const;

    public slots:
        void setScan(bool scan);
        void setModifierKey(int key);
        void setShowIfNotFound(bool mode);
        void setTranslationFlags(DictCore::TranslationFlags translationFlags);
        void showTranslation(const QString &text);

    private slots:
        void selectionChanged(const QString &text);

    signals:
        void scanChanged(bool scan);

    private:
        DictWidget *translationView;
        DictCore* m_dict;
        int m_modifierKey;
        bool m_showIfNotFound;
        Selection *m_selection;
};

#endif // POPUPWINDOW_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

