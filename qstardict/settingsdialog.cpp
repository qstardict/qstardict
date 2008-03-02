/*****************************************************************************
 * settingsdialog.cpp - QStarDict, a StarDict clone written with using Qt    *
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

#include "settingsdialog.h"

#include <QFileDialog>
#include "dictcore.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "adddictionarydialog.h"
#include "application.h"

namespace QStarDict {

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    setupUi(this);

    // Load popup window settings
    PopupWindow *popup = Application::instance()->popupWindow();
    useScanBox->setChecked(popup->isScan());
    if (popup->modifierKey())
    {
        useScanModifierBox->setChecked(true);
        QString modifierName;
        switch (popup->modifierKey())
        {
        case Qt::ShiftModifier:
            modifierName = "Shift";
            break;
        case Qt::ControlModifier:
            modifierName = "Control";
            break;
        case Qt::AltModifier:
            modifierName = "Alt";
            break;
        case Qt::MetaModifier:
            modifierName = "Win";
            break;
        }
        modifierKeyBox->setCurrentIndex(modifierKeyBox->findText(modifierName));
    }
    showIfNotFoundBox->setChecked(popup->showIfNotFound());
    popupOpacitySpin->setValue(static_cast<int>(popup->windowOpacity() * 100));
    timeoutBeforeHideSpin->setValue(popup->timeoutBeforeHide() / 1000.0);
    popupDefaultWidthSpin->setValue(popup->defaultSize().width());
    popupDefaultHeightSpin->setValue(popup->defaultSize().height());
    pronounceWordBox->setChecked(popup->pronounceWord());
//    speechProgramEdit->setText(parent->popup->speechProgram());
//    instantSearchBox->setChecked(parent->isInstantSearch());

#if 0
    connect(moveUpOrderedDictsButton, SIGNAL(clicked()), SLOT(moveUpOrderedDictsButtonClicked()));
    connect(moveDownOrderedDictsButton, SIGNAL(clicked()), SLOT(moveDownOrderedDictsButtonClicked()));
    connect(moveLeftOrderedDictsButton, SIGNAL(clicked()), SLOT(moveLeftOrderedDictsButtonClicked()));
    connect(moveRightOrderedDictsButton, SIGNAL(clicked()), SLOT(moveRightOrderedDictsButtonClicked()));
    connect(addDictDirButton, SIGNAL(clicked()), SLOT(addDictDirsButtonClicked()));
    connect(removeDictDirButton, SIGNAL(clicked()), SLOT(removeDictDirsButtonClicked()));
    connect(moveUpDictDirButton, SIGNAL(clicked()), SLOT(moveUpDictDirsButtonClicked()));
    connect(moveDownDictDirButton, SIGNAL(clicked()), SLOT(moveDownDictDirsButtonClicked()));
#endif

    connect(this, SIGNAL(accepted()), SLOT(apply()));
}

void SettingsDialog::apply()
{
#if 0
    QStringList dirs;
    QStringList ordered;

    for (int i = 0; i < dictDirsList->count(); ++i)
        dirs << dictDirsList->item(i)->text();
    mainWindow->m_dict->setDictDirs(dirs);
    for (int i = 0; i < orderedDictsList->count(); ++i)
        ordered << orderedDictsList->item(i)->text();
    mainWindow->m_dict->setDicts(ordered);
#endif

    // Save the popup window settings
    PopupWindow *popup = Application::instance()->popupWindow();
    popup->setScan(useScanBox->isChecked());
    int modifierKey = 0;
    if (useScanModifierBox->isChecked())
        if (modifierKeyBox->currentText() == "Shift")
            modifierKey = Qt::ShiftModifier;
        else if (modifierKeyBox->currentText() == "Control")
            modifierKey = Qt::ControlModifier;
        else if (modifierKeyBox->currentText() == "Alt")
            modifierKey = Qt::AltModifier;
        else if (modifierKeyBox->currentText() == "Win")
            modifierKey = Qt::MetaModifier;
    popup->setShowIfNotFound(showIfNotFoundBox->isChecked());
    popup->setModifierKey(modifierKey);
    popup->setWindowOpacity(popupOpacitySpin->value() / 100.0);
    popup->setTimeoutBeforeHide(static_cast<int>(timeoutBeforeHideSpin->value() * 1000.0));
    popup->setDefaultSize(QSize(popupDefaultWidthSpin->value(), popupDefaultHeightSpin->value()));
#if 0
    DictCore::TranslationFlags translationFlags = mainWindow->translationView->translationFlags();
    if (reformatTranslationsBox->isChecked())
        translationFlags |= DictCore::Reformat;
    else
        translationFlags &= ~DictCore::Reformat;
    if (expandAbbreviationsBox->isChecked())
        translationFlags |= DictCore::ExpandAbbreviations;
    else
        translationFlags &= ~DictCore::ExpandAbbreviations;
    mainWindow->translationView->setTranslationFlags(translationFlags);
    mainWindow->popup->setTranslationFlags(translationFlags);
    mainWindow->popup->setPronounceWord(pronounceWordBox->isChecked());
    mainWindow->popup->setSpeechProgram(speechProgramEdit->text());
    mainWindow->setInstantSearch(instantSearchBox->isChecked());

    mainWindow->on_queryButton_clicked();
#endif
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

