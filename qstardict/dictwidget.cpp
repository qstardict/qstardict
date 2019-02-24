/*****************************************************************************
 * dictwidget.cpp - QStarDict, a StarDict clone written with using Qt        *
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

#include "dictwidget.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include "application.h"
#include "dictbrowser.h"
#include "dictbrowsersearch.h"
#include "speaker.h"
#include "pluginmanager.h"
#include "../plugins/toolbarplugin.h"

namespace
{
class DictWidgetToolbar: public QToolBar
{
    public:
        DictWidgetToolbar(QWidget *parent = 0)
            : QToolBar(parent)
        { }

    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent *event)
        {
            if (! actionAt(event->pos()))
                QToolBar::mouseDoubleClickEvent(event);
        }
};
}

namespace QStarDict
{

DictWidget::DictWidget(QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    m_translationView = new DictBrowser(this);
    setFrameStyle(m_translationView->frameStyle());
    m_translationView->setFrameStyle(QFrame::NoFrame);
    m_translationView->verticalScrollBar()->setCursor(Qt::ArrowCursor);
    m_translationView->horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    m_translationView->setOpenExternalLinks(true);
    connect(m_translationView, SIGNAL(sourceChanged(const QUrl&)), SLOT(on_translationView_sourceChanged(const QUrl&)));

    m_search = new DictBrowserSearch(this);
    connect(m_search, SIGNAL(search(const QString &,QTextDocument::FindFlags)),
            m_translationView, SLOT(search(const QString &,QTextDocument::FindFlags)));
    connect(m_translationView, SIGNAL(searchResult(bool)), m_search, SLOT(searchResult(bool)));
    m_search->hide();

    m_toolBar = new DictWidgetToolbar(this);
    m_toolBar->setMouseTracking(true);

    QAction *actionBackward = m_toolBar->addAction(QIcon(":/icons/go-previous.png"), tr("Go to &previous translation"),
            m_translationView, SLOT(backward()));
    actionBackward->setDisabled(true);
    connect(m_translationView, SIGNAL(backwardAvailable(bool)), actionBackward, SLOT(setEnabled(bool)));

    QAction *actionForward = m_toolBar->addAction(QIcon(":/icons/go-next.png"), tr("Go to &next translation"),
            m_translationView, SLOT(forward()));
    actionForward->setDisabled(true);
    connect(m_translationView, SIGNAL(forwardAvailable(bool)), actionForward, SLOT(setEnabled(bool)));

    m_toolBar->addAction(QIcon(":/icons/document-save-as.png"), tr("&Save to file"),
            this, SLOT(saveToFile()));

    m_toolBar->addAction(QIcon(":/icons/document-print.png"), tr("Prin&t translation"),
            this, SLOT(print()));

    m_toolBar->addAction(QIcon(":/icons/speaker.png"), tr("Speak &word"),
            this, SLOT(speak()));

    QAction *actionSearch = m_toolBar->addAction(QIcon(":/icons/system-search.png"), tr("Search"), this, SLOT(handleSearch()));
    actionSearch->setCheckable(true);
    actionSearch->setShortcut(QKeySequence::Find);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_toolBar);
    layout->addWidget(m_translationView);
    layout->addWidget(m_search);
    setLayout(layout);

    connect(m_toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(pluginAction(QAction*)));

    reloadToolbar();
}

void DictWidget::reloadToolbar() {
    for (auto i = m_toolbarPlugins.constBegin(); i != m_toolbarPlugins.constEnd(); i++) {
        m_toolBar->removeAction(i.key());
        delete i.key();
    }
    m_toolbarPlugins.clear();

    auto pm = Application::instance()->pluginManager();
    foreach (const QString &pluginId, pm->availablePlugins()) {
        auto plugin = pm->plugin<ToolbarPlugin>(pluginId);
        if (plugin) {
            QAction *action = m_toolBar->addAction(plugin->toolbarIcon(), plugin->toolbarText());
            m_toolbarPlugins[action] = plugin;
        }
    }
}

void DictWidget::pluginAction(QAction *action) {
    if (m_toolbarPlugins.find(action) != m_toolbarPlugins.end()) {
        m_toolbarPlugins[action]->execute(this);
    }
}

void DictWidget::translate(const QString &str)
{
    m_translationView->setSource(QUrl("qstardict:" + str));
}

void DictWidget::on_translationView_sourceChanged(const QUrl &name)
{
    emit wordTranslated(name.toString(QUrl::RemoveScheme));
}

void DictWidget::saveToFile()
{
    static QDir dir( QDir::homePath() ); //added by Frank
    static QString filter(tr("Text files (*.txt)")); //added by Frank

    QFileDialog dialog(this, tr("Save translation"),
                       dir.path(), filter); //updated by Frank
    dialog.selectFile(translatedWord());//added by Frank
    dialog.setNameFilters(QStringList() << tr("HTML files (*.html *.htm)") << tr("Text files (*.txt)"));//updated by Frank
    dialog.selectNameFilter(filter); //added by Frank

    if (dialog.exec() && dialog.selectedFiles().size())
    {
        QString fileName = dialog.selectedFiles().first();
        /*QString*/ filter = dialog.selectedNameFilter();//updated by Frank
        dir = dialog.directory(); //added by Frank
        if (filter == tr("HTML files (*.html, *.htm)") &&
            ! (fileName.endsWith(".html", Qt::CaseInsensitive) || fileName.endsWith(".htm", Qt::CaseInsensitive)))
            fileName += ".html";
        else if (filter == tr("Text files (*.txt)") && ! fileName.endsWith(".txt", Qt::CaseInsensitive))
            fileName += ".txt";

        QFile outputFile(fileName);
        if (! outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Cannot save translation as %1").arg(fileName));
            return;
        }
        QTextStream outputStream(&outputFile);
        if (filter == tr("HTML files (*.html, *.htm)"))
            outputStream << m_translationView->document()->toHtml("UTF-8");
        else
            outputStream << m_translationView->toPlainText();
    }
}

void DictWidget::speak()
{
    Application::instance()->speaker()->speak(translatedWord());
}

void DictWidget::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted)
        m_translationView->print(&printer);
}

void DictWidget::handleSearch()
{
    m_search->setVisible(!m_search->isVisible());
}

void DictWidget::setDefaultStyleSheet(const QString &css)
{
    m_translationView->document()->setDefaultStyleSheet(css);
    m_translationView->reload();
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

