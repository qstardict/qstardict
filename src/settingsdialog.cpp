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

SettingsDialog::SettingsDialog(MainWindow *parent)
        : QDialog(parent)
{
    setupUi(this);
    mainWindow = parent;

    orderedDictsList->addItems(parent->m_dict->orderedDicts());
    disabledDictsList->addItems(parent->m_dict->disabledDicts());
    dictDirsList->addItems(parent->m_dict->dictDirs());

    useScanBox->setChecked(parent->popup->isScan());
    if (parent->popup->modifierKey())
    {
        useScanModifierBox->setChecked(true);
        QString modifierName;
        switch (parent->popup->modifierKey())
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
    showIfNotFoundBox->setChecked(parent->popup->showIfNotFound());
    popupOpacitySpin->setValue(static_cast<int>(parent->popup->windowOpacity() * 100));
    reformatTranslationsBox->setChecked(parent->translationView->translationFlags().testFlag(DictCore::Reformat));
    reformatTranslationsWarningLabel->setVisible(reformatTranslationsBox->isChecked());
    expandAbbreviationsBox->setChecked(parent->translationView->translationFlags().testFlag(
                DictCore::ExpandAbbreviations));
    timeoutBeforeHideSpin->setValue(parent->popup->timeoutBeforeHide() / 1000.0);
    popupDefaultWidthSpin->setValue(parent->popup->defaultSize().width());
    popupDefaultHeightSpin->setValue(parent->popup->defaultSize().height());
    pronounceWordBox->setChecked(parent->popup->pronounceWord());
    speechProgramEdit->setText(parent->popup->speechProgram());
    instantSearchBox->setChecked(parent->isInstantSearch());

    connect(moveUpOrderedDictsButton, SIGNAL(clicked()), SLOT(moveUpOrderedDictsButtonClicked()));
    connect(moveDownOrderedDictsButton, SIGNAL(clicked()), SLOT(moveDownOrderedDictsButtonClicked()));
    connect(moveLeftOrderedDictsButton, SIGNAL(clicked()), SLOT(moveLeftOrderedDictsButtonClicked()));
    connect(moveRightOrderedDictsButton, SIGNAL(clicked()), SLOT(moveRightOrderedDictsButtonClicked()));
    connect(addDictDirButton, SIGNAL(clicked()), SLOT(addDictDirsButtonClicked()));
    connect(removeDictDirButton, SIGNAL(clicked()), SLOT(removeDictDirsButtonClicked()));
    connect(moveUpDictDirButton, SIGNAL(clicked()), SLOT(moveUpDictDirsButtonClicked()));
    connect(moveDownDictDirButton, SIGNAL(clicked()), SLOT(moveDownDictDirsButtonClicked()));

    connect(this, SIGNAL(accepted()), SLOT(apply()));
}

void SettingsDialog::updateOrder()
{
    QStringList newDictsDirs;

    for (int i = 0; i < dictDirsList->count(); ++i)
    {
        newDictsDirs << DictCore::findDicts(dictDirsList->item(i)->text());
    }

    for (int i = 0; i < orderedDictsList->count(); ++i)
    {
        int index = newDictsDirs.indexOf(orderedDictsList->item(i)->text());

        if (index != -1)
            newDictsDirs.removeAt(index);
        else
            delete orderedDictsList->takeItem(i--);
    }

    disabledDictsList->clear();
    disabledDictsList->addItems(newDictsDirs);
}

void SettingsDialog::apply()
{
    QStringList dirs;
    QStringList ordered;

    for (int i = 0; i < dictDirsList->count(); ++i)
        dirs << dictDirsList->item(i)->text();
    mainWindow->m_dict->setDictDirs(dirs);
    for (int i = 0; i < orderedDictsList->count(); ++i)
        ordered << orderedDictsList->item(i)->text();
    mainWindow->m_dict->setDicts(ordered);

    mainWindow->popup->setScan(useScanBox->isChecked());
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
    mainWindow->popup->setShowIfNotFound(showIfNotFoundBox->isChecked());
    mainWindow->popup->setModifierKey(modifierKey);
    mainWindow->popup->setWindowOpacity(popupOpacitySpin->value() / 100.0);
    mainWindow->popup->setTimeoutBeforeHide(static_cast<int>(timeoutBeforeHideSpin->value() * 1000.0));
    mainWindow->popup->setDefaultSize(QSize(popupDefaultWidthSpin->value(), popupDefaultHeightSpin->value()));
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

    mainWindow->queryButtonClicked();
}

void SettingsDialog::moveUpOrderedDictsButtonClicked()
{
    if (orderedDictsList->currentRow() > 0)
    {
        orderedDictsList->insertItem(orderedDictsList->currentRow(),
                                     orderedDictsList->takeItem(orderedDictsList->currentRow()));
        orderedDictsList->setCurrentRow(orderedDictsList->currentRow() - 1);
    }
}

void SettingsDialog::moveDownOrderedDictsButtonClicked()
{
    if (orderedDictsList->currentRow() < orderedDictsList->count() - 1)
        orderedDictsList->insertItem(orderedDictsList->currentRow(),
                                     orderedDictsList->takeItem(orderedDictsList->currentRow() + 1));
}

void SettingsDialog::moveUpDictDirsButtonClicked()
{
    if (dictDirsList->currentRow() > 0)
    {
        dictDirsList->insertItem(dictDirsList->currentRow(),
                                 dictDirsList->takeItem(dictDirsList->currentRow()));
        dictDirsList->setCurrentRow(dictDirsList->currentRow() - 1);
    }
}

void SettingsDialog::moveDownDictDirsButtonClicked()
{
    if (dictDirsList->currentRow() < dictDirsList->count() - 1)
        dictDirsList->insertItem(dictDirsList->currentRow(),
                                 dictDirsList->takeItem(dictDirsList->currentRow() + 1));
}

void SettingsDialog::moveLeftOrderedDictsButtonClicked()
{
    disabledDictsList->addItem(orderedDictsList->takeItem(orderedDictsList->currentRow()));
}

void SettingsDialog::moveRightOrderedDictsButtonClicked()
{
    orderedDictsList->addItem(disabledDictsList->takeItem(disabledDictsList->currentRow()));
}

void SettingsDialog::addDictDirsButtonClicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select dictionaries directory"));
    if (! dirName.isEmpty())
    {
        dictDirsList->addItem(dirName);
        updateOrder();
    }
}

void SettingsDialog::removeDictDirsButtonClicked()
{
    delete dictDirsList->takeItem(dictDirsList->currentRow());
    updateOrder();
}
