/*****************************************************************************
 * popupwindow.cpp - QStarDict, a StarDict clone written with using Qt       *
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

#include "popupwindow.h"

#include <QGridLayout>
#include <QProcess>
#include <QSettings>
#include "dictwidget.h"
#include "keyboard.h"
#include "selection.h"

PopupWindow::PopupWindow(DictCore *dict, QWidget *parent)
        : ResizablePopup(parent)
{
    if (! dict)
        m_dict = new DictCore(this);
    else
        m_dict = dict;
    translationView = new DictWidget(this);
    translationView->setFrameStyle(QFrame::NoFrame);
    translationView->setDict(m_dict);
    translationView->setMouseTracking(true);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(translationView);
    m_speechProcess = new QProcess(this);

    m_selection = new Selection(this);
    connect(m_selection, SIGNAL(changed(const QString&)), this, SLOT(selectionChanged(const QString&)));

    loadSettings();
}

PopupWindow::~PopupWindow()
{
    saveSettings();
}

void PopupWindow::loadSettings()
{
    QSettings config;
    setScan(config.value("PopupWindow/scan", true).toBool());
    setModifierKey(config.value("PopupWindow/modifierKey", 0).toInt());
    setShowIfNotFound(config.value("PopupWindow/showIfNotFound", false).toBool());
    setWindowOpacity(config.value("PopupWindow/opacity", 1.0).toDouble());
    setTimeoutBeforeHide(config.value("PopupWindow/timeoutBeforeHide", 500).toInt());
    setDefaultSize(config.value("PopupWindow/defaultSize", QSize(320, 240)).toSize());
    setTranslationFlags(static_cast<DictCore::TranslationFlags>(config.value("DictWidget/translationFlags",
                    static_cast<int>(translationView->translationFlags())).toInt()));
    setPronounceWord(config.value("PopupWindow/pronounceWord", true).toBool());
    setSpeechProgram(config.value("PopupWindow/speechProgram", "festival --tts").toString());
}

void PopupWindow::saveSettings()
{
    QSettings config;
    config.setValue("PopupWindow/scan", isScan());
    config.setValue("PopupWindow/modifierKey", m_modifierKey);
    config.setValue("PopupWindow/showIfNotFound", m_showIfNotFound);
    config.setValue("PopupWindow/opacity", windowOpacity());
    config.setValue("PopupWindow/timeoutBeforeHide", timeoutBeforeHide());
    config.setValue("PopupWindow/defaultSize", defaultSize());
    config.setValue("PopupWindow/pronounceWord", pronounceWord());
}

void PopupWindow::setScan(bool scan)
{
    m_selection->setScan(scan);
    emit scanChanged(scan);
}

bool PopupWindow::isScan() const
{
    return m_selection->isScan();
}

void PopupWindow::selectionChanged(const QString &text)
{
    if (m_modifierKey && ! Keyboard::activeModifiers().testFlag(static_cast<Qt::KeyboardModifier>(m_modifierKey)))
        return;
    showTranslation(text);
}

void PopupWindow::showTranslation(const QString &text)
{
    bool isFound = m_dict->isTranslatable(text);
    if (m_showIfNotFound || isFound)
    {
	QString sourceText = text.simplified();
	translationView->translate(sourceText);
	popup();
	if (isFound && m_pronounceWord)
	{
	    if (m_speechProcess->state() != QProcess::NotRunning)
		m_speechProcess->kill();
	    
	    m_speechProcess->start(m_speechProgram, QIODevice::WriteOnly);
	    if (! m_speechProcess->waitForStarted())
		return;
	    m_speechProcess->write(sourceText.toUtf8());
	    m_speechProcess->closeWriteChannel();
	}
    }
}

void PopupWindow::setTranslationFlags(DictCore::TranslationFlags translationFlags)
{
    translationView->setTranslationFlags(translationFlags | DictCore::Simple);
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

