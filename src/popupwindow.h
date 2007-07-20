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

#include <QFrame>
#include "dictcore.h"

class QAction;
class QEvent;
class QTimer;
class QClipboard;

class DictWidget;

class PopupWindow: public QFrame
{
    Q_OBJECT

    public:
        PopupWindow(DictCore *dict = NULL, QWidget *parent = 0);
        ~PopupWindow();

        bool isScan() const;
        int modifierKey() const;
        bool showIfNotFound() const;
        int timeoutBeforeHide() const;
        const QSize& defaultSize() const;
        DictCore::TranslationFlags translationFlags() const;

        DictCore* dict() const;

    public slots:
        void setScan(bool scan);
        void setModifierKey(int key);
        void setShowIfNotFound(bool mode);
        void setTimeoutBeforeHide(int timeoutBeforeHide);
        void setDefaultSize(const QSize &defaultSize);
        void setTranslationFlags(DictCore::TranslationFlags translationFlags);
        void showTranslation(const QString &text);

    signals:
        void scanChanged(bool);

    protected:
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
        void timerEvent(QTimerEvent*);
    
    private slots:
        void xSelectionChanged();

    private:
        DictCore* m_dict;
        DictWidget *translationView;
        QTimer *closeTimer;
        QString m_source;
        bool m_scan;
        int m_modifierKey;
        bool m_showIfNotFound;
        QString lastSelection;
        int timerId;
        int m_timeoutBeforeHide;
        QSize m_defaultSize;
};

#endif // POPUPWINDOW_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

