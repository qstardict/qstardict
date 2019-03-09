/*****************************************************************************
 * dictwidget.h - QStarDict, a quasi-star dictionary                         *
 * Copyright (C) 2007-2019 Alexander Rodin                                   *
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
#include "dictbrowser.h"

class QToolBar;
class QAction;

namespace QStarDict
{

class DictBrowserSearch;
class ToolbarPlugin;

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
         * Warning: DictWidget will copy only a pointer to dict. So set dictionaries
         * allocated from heap and don't destroy it befor DictWidget.
         */
        void setDict(DictCore *dict)
        { m_translationView->setDict(dict); }
        /**
         * Return pointer to dictionary.
         */
        const DictCore* dict() const
        { return m_translationView->dict(); }
        /**
         * Clear translation text.
         */
        void clear()
        { m_translationView->clear(); }

        /**
         * Clear history.
         */
        void clearHistory()
        { m_translationView->clearHistory(); }

        /**
         * Show translation of str.
         */
        void translate(const QString &str);
        /**
         * Return last translated word.
         */
        QString translatedWord() const
        { return m_translationView->source().toString(QUrl::RemoveScheme); }

        /**
         * Return toolbar.
         */
        QToolBar *toolBar()
        { return m_toolBar; }

        void setDefaultStyleSheet(const QString &css);

        QString defaultStyleSheet() const
        { return m_translationView->document()->defaultStyleSheet(); }

        void setShowLinks(bool showLinks)
        { m_translationView->setShowLinks(showLinks); }
        bool showLinks() const
        { return m_translationView->showLinks(); }

        void setShowLinksModifierKey(int key)
        { m_translationView->setShowLinksModifierKey(key); }
        int showLinksModifierKey() const
        { return m_translationView->showLinksModifierKey(); }

        void reload()
        { m_translationView->reload(); }

        DictBrowser *translationView() const
        { return m_translationView; }

        void reloadToolbar();

    signals:
        /**
         * Emits when translated word is shown.
         */
        void wordTranslated(const QString &word);

    private slots:
        void on_translationView_sourceChanged(const QUrl &name);
        void speak();
        void handleSearch();
        void pluginAction(QAction *action);

    private:
        DictBrowser *m_translationView;
        QToolBar *m_toolBar;
        QHash<QAction*, ToolbarPlugin*> m_toolbarPlugins;
        DictBrowserSearch *m_search;
};

}

#endif // DICTWIDGET_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

