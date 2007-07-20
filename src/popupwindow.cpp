/********************************************************************************
 * popupwindow.cpp - QStarDict, a StarDict clone written with using Qt          *
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

#include "popupwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QSettings>
#include <QTimer>
#include "dictwidget.h"
#include "keyboard.h"

PopupWindow::PopupWindow(DictCore *dict, QWidget *parent)
        : QFrame(parent, Qt::ToolTip)
{
    if (! dict)
        m_dict = new DictCore(this);
    else
        m_dict = dict;
    setFrameStyle(QFrame::Box);
    translationView = new DictWidget(this);
    translationView->setDict(m_dict);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(translationView);
    closeTimer = new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), SLOT(close()));
    connect(closeTimer, SIGNAL(timeout()), closeTimer, SLOT(stop()));
    timerId = 0;

    QSettings config;
    setScan(config.value("PopupWindow/scan", true).toBool());
    setModifierKey(config.value("PopupWindow/modifierKey", 0).toInt());
    setShowIfNotFound(config.value("PopupWindow/showIfNotFound", false).toBool());
    setWindowOpacity(config.value("PopupWindow/opacity", 1.0).toDouble());
    m_timeoutBeforeHide = config.value("PopupWindow/timeoutBeforeHide", 300).toInt();
    m_defaultSize = config.value("PopupWindow/defaultSize", QSize(320, 240)).toSize();
    setTranslationFlags(DictCore::TranslationFlags(config.value("DictWidget/translationFlags", (int)translationView->translationFlags()).toInt()));
}

PopupWindow::~PopupWindow()
{
    QSettings config;
    config.setValue("PopupWindow/scan", m_scan);
    config.setValue("PopupWindow/modifierKey", m_modifierKey);
    config.setValue("PopupWindow/showIfNotFound", m_showIfNotFound);
    config.setValue("PopupWindow/opacity", windowOpacity());
    config.setValue("PopupWindow/timeoutBeforeHide", m_timeoutBeforeHide);
    config.setValue("PopupWindow/defaultSize", m_defaultSize);
}

void PopupWindow::setScan(bool scan)
{
    if (m_scan == scan)
        return ;
    m_scan = scan;
    if (m_scan)
    {
        lastSelection = QApplication::clipboard()->text(QClipboard::Selection);
        timerId = startTimer(300);
    }
    else
        killTimer(timerId);
    emit scanChanged(scan);
}

bool PopupWindow::isScan() const
{
    return m_scan;
}

void PopupWindow::setModifierKey(int key)
{
    m_modifierKey = key;
}

int PopupWindow::modifierKey() const
{
    return m_modifierKey;
}

bool PopupWindow::showIfNotFound() const
{
    return m_showIfNotFound;
}

void PopupWindow::setShowIfNotFound(bool mode)
{
    m_showIfNotFound = mode;
}

DictCore* PopupWindow::dict() const
{
    return m_dict;
}

void PopupWindow::timerEvent(QTimerEvent*)
{
    if (lastSelection != QApplication::clipboard()->text(QClipboard::Selection))
    {
        lastSelection = QApplication::clipboard()->text(QClipboard::Selection);
        xSelectionChanged();
    }
}

void PopupWindow::xSelectionChanged()
{
    if (m_modifierKey && ! Keyboard::activeModifiers().testFlag(static_cast<Qt::KeyboardModifier>(m_modifierKey)))
        return ;
    QString text = QApplication::clipboard()->text(QClipboard::Selection);
    text.remove(QRegExp("^\\W"));
    text.remove(QRegExp("\\W.*$"));
    if (m_showIfNotFound || m_dict->isTranslatable(text))
        showTranslation(text);
}

void PopupWindow::showTranslation(const QString &text)
{
    m_source = text;
    translationView->translate(m_source);
    resize(m_defaultSize);

    QPoint newPosition = cursor().pos() - QPoint(30, 30);
    if (newPosition.x() < 0)
        newPosition.setX(0);
    else if (newPosition.x() + width() > QApplication::desktop()->width())
        newPosition.setX(QApplication::desktop()->width() - width());
    if (newPosition.y() < 0)
        newPosition.setY(0);
    else if (newPosition.y() + height() > QApplication::desktop()->height())
        newPosition.setY(QApplication::desktop()->height() - height());
    move(newPosition);
    show();
}

void PopupWindow::enterEvent(QEvent*)
{
    closeTimer->stop();
}

void PopupWindow::leaveEvent(QEvent*)
{
    closeTimer->start(m_timeoutBeforeHide);
}

int PopupWindow::timeoutBeforeHide() const
{
    return m_timeoutBeforeHide;
}

void PopupWindow::setTimeoutBeforeHide(int timeoutBeforeHide)
{
    m_timeoutBeforeHide = timeoutBeforeHide;
}

const QSize& PopupWindow::defaultSize() const
{
    return m_defaultSize;
}

void PopupWindow::setDefaultSize(const QSize &defaultSize)
{
    m_defaultSize = defaultSize;
}

void PopupWindow::setTranslationFlags(DictCore::TranslationFlags translationFlags)
{
    translationView->setTranslationFlags(translationFlags | DictCore::Simple);
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

