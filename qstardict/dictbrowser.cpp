/*****************************************************************************
 * dictbrowser.cpp - QStarDict, a quasi-star dictionary                      *
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
#include <QtDebug>
#include "dictbrowser.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include "../plugins/dictplugin.h"
#include "application.h"
#include "pluginmanager.h"
#include "keyboard.h"

namespace
{
const QString translationCSS = 
    "body {\n"
#ifdef Q_OS_MAC
        "font-size: 13pt; font-family: Avenir; }\n"
#else
        "font-size: 10pt; }\n"
#endif
    "font.dict_name {\n"
        "color: blue;\n"
        "font-style: italic; }\n"
    "font.title {\n"
        "font-size: 16pt;\n"
        "font-weight: bold; }\n"
    "font.explanation {\n"
        "color: #7f7f7f;\n"
        "font-style: italic; }\n"
    "font.abbreviature {\n"
        "font-style: italic; }\n"
    "font.example {\n"
        "font-style: italic; }\n"
    "font.transcription {\n"
        "font-weight: bold; }\n";
}

namespace QStarDict
{

DictBrowser::DictBrowser(QWidget *parent)
    : QTextBrowser(parent),
      m_dict(0),
      m_highlighted(false),
      m_highlightTimerId(0),
      m_showLinks(true),
      m_showLinksModifierKey(0),
      m_highlightInCurrentTranslation(false)
{
    document()->setDefaultStyleSheet(translationCSS);
    setOpenLinks(false);
    setOpenExternalLinks(false);
    connect(this, SIGNAL(anchorClicked(const QUrl &)), SLOT(on_anchorClicked(const QUrl &)));
    Application::instance()->installEventFilter(this);
}

QVariant DictBrowser::loadResource(int type, const QUrl &name)
{
    m_highlightInCurrentTranslation = false;
    if (type == QTextDocument::HtmlResource && name.scheme() == "qstardict")
    {
        QString str = name.toString(QUrl::RemoveScheme);
        QString result = m_dict->translate(str);
        if (result.isEmpty())
            result = "<table><tr><td><img src=\":/icons/dialog-warning.png\" width=64 height=64/></td><td valign=middle>" +
                tr("The word <b>%1</b> is not found.").arg(str) +
                "</td></tr></table>";
        return "<title>Translation for \"" + str + "\"</title>\n"
            + "<body>" + result + "</body>";
    }
    else if (name.scheme() == "plugin")
    {
        DictPlugin *plugin = Application::instance()->pluginManager()->plugin<DictPlugin>(name.host());
        if (! plugin)
            return QVariant();
        return plugin->resource(type, name);
    }
    return QTextBrowser::loadResource(type, name);
}

void DictBrowser::search(const QString & exp, QTextDocument::FindFlags options)
{
    bool found = false;

    QList<QTextEdit::ExtraSelection> extraSelections;

    moveCursor(QTextCursor::Start);
    QColor color = QColor(Qt::gray).lighter(130);

    while (find(exp, options))
    {
        found = true;

        QTextEdit::ExtraSelection extra;
        extra.format.setBackground(color);

        extra.cursor = textCursor();
        extraSelections.append(extra);
    }

    setExtraSelections(extraSelections);

    emit searchResult(found);
}

void DictBrowser::searchActive(bool active)
{
    if (!active)
    {
        moveCursor(QTextCursor::Start);
        setExtraSelections({});
    }
}

void DictBrowser::invalidateHighlight()
{
    auto overrideCursor = false;
    if (m_highlighted) // clear highlight if any
    {
        m_oldCursor.setCharFormat(m_oldFormat);
        m_highlighted = false;
        killTimer(m_highlightTimerId);
        m_highlightTimerId = 0;
        m_highlightedWord.clear();
    }
    if (!m_highlightInCurrentTranslation)
    {
        QApplication::restoreOverrideCursor();
        return;
    }

    QPoint mousePosition = mapFromGlobal(QCursor::pos());
    if (areLinksActive() && contentsRect().contains(mousePosition) && wordRect(mousePosition).contains(mousePosition))
    {
        // highlight word if found
        auto cursor = cursorForPosition(mousePosition);
        cursor.select(QTextCursor::WordUnderCursor);
        QString selection = cursor.selection().toPlainText().simplified();
        if (selection == m_highlightedWord || m_dict->isTranslatable(selection))
        {
            m_oldCursor = cursor;
            m_oldFormat = cursor.charFormat();

            QTextCharFormat format = m_oldFormat;
            format.setForeground(Qt::blue);
            format.setFontUnderline(true);
            cursor.setCharFormat(format);

            m_highlighted = true;
            m_highlightedWord = selection;
            m_highlightTimerId = startTimer(100);
            overrideCursor = true;
        }
    }

    if (overrideCursor)
    {
        if (!QApplication::overrideCursor())
            QApplication::setOverrideCursor(Qt::PointingHandCursor);
    }
    else
        QApplication::restoreOverrideCursor();
}

QRect DictBrowser::wordRect(const QPoint &mousePosition)
{
    auto cursor = cursorForPosition(mousePosition);
    cursor.select(QTextCursor::WordUnderCursor);
    auto selectionStart = cursor.selectionStart();
    auto selectionEnd = cursor.selectionEnd();

    cursor.setPosition(selectionStart);
    auto topLeft = cursorRect(cursor).topLeft();
    cursor.setPosition(selectionEnd);
    auto bottomRight = cursorRect(cursor).bottomRight();

    return QRect(topLeft, bottomRight);
}

void DictBrowser::mouseMoveEvent(QMouseEvent *event)
{
    if (areLinksActive())
    {
        m_highlightInCurrentTranslation = true;
        invalidateHighlight();
    }
    QTextBrowser::mouseMoveEvent(event);
}

bool DictBrowser::areLinksActive()
{
    if (!m_showLinks)
        return false;
    if (m_showLinksModifierKey == 0)
        return true;
    return Keyboard::activeModifiers().testFlag(static_cast<Qt::KeyboardModifier>(m_showLinksModifierKey));
}

void DictBrowser::mouseReleaseEvent(QMouseEvent *event)
{
    if (areLinksActive())
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString selection = cursor.selection().toPlainText().simplified();
        if (m_dict->isTranslatable(selection) &&
            selection != source().toString(QUrl::RemoveScheme) &&
            !textCursor().hasSelection())
            setSource(selection);
    }
    QTextBrowser::mousePressEvent(event);
}

void DictBrowser::on_anchorClicked(const QUrl &link)
{
    QString scheme = link.scheme();
    if (scheme == "plugin" || scheme == "qrc")
        setSource(link);
    else
        QDesktopServices::openUrl(link);
}

void DictBrowser::timerEvent(QTimerEvent*)
{
    invalidateHighlight();
}

bool DictBrowser::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        if (m_showLinksModifierKey)
            m_highlightInCurrentTranslation = true;
        invalidateHighlight();
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        invalidateHighlight();
    }
    return false;
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

