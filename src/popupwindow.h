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

class QProcess;
class DictWidget;
class Selection;

class PopupWindow: public ResizablePopup
{
    Q_OBJECT

    public:
        PopupWindow(DictCore *dict = 0, QWidget *parent = 0);
        ~PopupWindow();

        bool isScan() const;
        int modifierKey() const
        { return m_modifierKey; }
        bool showIfNotFound() const
        { return m_showIfNotFound; }
        DictCore::TranslationFlags translationFlags() const;
        DictCore* dict() const
        { return m_dict; }
	bool pronounceWord() const
	{ return m_pronounceWord; }
	const QString &speechProgram() const
        { return m_speechProgram; }

    public slots:
        void setScan(bool scan);
        void setModifierKey(int key)
        { m_modifierKey = key; }
        void setShowIfNotFound(bool mode)
        { m_showIfNotFound = mode; }
        void setTranslationFlags(DictCore::TranslationFlags translationFlags);
        void showTranslation(const QString &text);
	void setPronounceWord(bool pronounceWord)
        { m_pronounceWord = pronounceWord; }
	void setSpeechProgram(const QString &speechProgram)
        { m_speechProgram = speechProgram; }

    private slots:
        void selectionChanged(const QString &text);

    signals:
        void scanChanged(bool scan);

    private:
        void loadSettings();
        void saveSettings();

        DictWidget *translationView;
        DictCore* m_dict;
        int m_modifierKey;
        bool m_showIfNotFound;
        Selection *m_selection;
	bool m_pronounceWord;
	QString m_speechProgram;
	QProcess *m_speechProcess;
};

#endif // POPUPWINDOW_H
