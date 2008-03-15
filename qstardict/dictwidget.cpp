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
#include <QTextDocument>
#include "dictbrowser.h"

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
    connect(m_translationView, SIGNAL(sourceChanged(const QUrl&)), SLOT(on_translationView_sourceChanged(const QUrl&)));

    m_toolBar = new QToolBar(this);
    m_actionBackward = m_toolBar->addAction(QIcon(":/icons/go-previous.png"), tr("Go to previous translation"),
            m_translationView, SLOT(backward()));
    m_actionBackward->setDisabled(true);
    connect(m_translationView, SIGNAL(backwardAvailable(bool)), m_actionBackward, SLOT(setEnabled(bool)));

    m_actionForward = m_toolBar->addAction(QIcon(":/icons/go-next.png"), tr("Go to next translation"),
            m_translationView, SLOT(forward()));
    m_actionForward->setDisabled(true);
    connect(m_translationView, SIGNAL(forwardAvailable(bool)), m_actionForward, SLOT(setEnabled(bool)));

    m_actionSaveToFile = m_toolBar->addAction(QIcon(":/icons/document-save-as.png"), tr("Save to file"),
            this, SLOT(saveToFile()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_toolBar);
    layout->addWidget(m_translationView);
    setLayout(layout);
}

void DictWidget::translate(const QString &str)
{
    m_translationView->setSource(str);
}

void DictWidget::on_translationView_sourceChanged(const QUrl &name)
{
    emit wordTranslated(name.toString());
}

void DictWidget::saveToFile()
{
    QFileDialog dialog(this, tr("Save translation"),
                       QDir::homePath() + "/" + translatedWord() + ".html");
    dialog.setFilters(QStringList() << tr("HTML files (*.html, *.htm)") << tr("Text files (*.txt)"));
    if (dialog.exec() && dialog.selectedFiles().size())
    {
        QString fileName = dialog.selectedFiles().first();
        QFile outputFile(fileName);
        if (! outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Cannot save translation as %1").arg(fileName));
            return;
        }
        QTextStream outputStream(&outputFile);
        QString filter = dialog.selectedFilter();
        if (filter == tr("HTML files (*.html, *.htm)"))
            outputStream << m_translationView->document()->toHtml("UTF-8");
        else
            outputStream << m_translationView->toPlainText();
    }
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

