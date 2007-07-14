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
    expandAbbreviationsBox->setChecked(parent->translationView->translationFlags().testFlag(DictCore::ExpandAbbreviations));
    timeoutBeforeHideSpin->setValue(parent->popup->timeoutBeforeHide() / 1000.0);

    connect(moveUpButton, SIGNAL(clicked()), SLOT(moveUpButtonClick()));
    connect(moveDownButton, SIGNAL(clicked()), SLOT(moveDownButtonClick()));
    connect(moveLeftButton, SIGNAL(clicked()), SLOT(moveLeftButtonClick()));
    connect(moveRightButton, SIGNAL(clicked()), SLOT(moveRightButtonClick()));
    connect(addDictDirButton, SIGNAL(clicked()), SLOT(addDictDirButtonClick()));
    connect(removeDictDirButton, SIGNAL(clicked()), SLOT(removeDictDirButtonClick()));
    connect(moveUpDictDirButton, SIGNAL(clicked()), SLOT(moveUpDictDirButtonClick()));
    connect(moveDownDictDirButton, SIGNAL(clicked()), SLOT(moveDownDictDirButtonClick()));

    connect(this, SIGNAL(accepted()), SLOT(apply()));
}

void SettingsDialog::updateOrder()
{
    QStringList newDictsDirs;

    for (int i = 0; i < dictDirsList->count(); i++)
    {
        newDictsDirs << DictCore::findDicts(dictDirsList->item(i)->text());
    }

    for (int i = 0; i < orderedDictsList->count(); i++)
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

    for (int i = 0; i < dictDirsList->count(); i++)
        dirs << dictDirsList->item(i)->text();
    mainWindow->m_dict->setDictDirs(dirs);
    for (int i = 0; i < orderedDictsList->count(); i++)
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
    DictCore::TranslationFlags translationFlags;
    if (reformatTranslationsBox->isChecked())
        translationFlags |= DictCore::Reformat;
    if (expandAbbreviationsBox->isChecked())
        translationFlags |= DictCore::ExpandAbbreviations;
    mainWindow->translationView->setTranslationFlags(translationFlags);

    mainWindow->queryButtonClicked();
}

void SettingsDialog::moveUpButtonClick()
{
    if (orderedDictsList->currentRow() > 0)
    {
        orderedDictsList->insertItem(orderedDictsList->currentRow() - 1,
                                     orderedDictsList->takeItem(orderedDictsList->currentRow()));
        orderedDictsList->setCurrentRow(orderedDictsList->currentRow() - 1);
    }
}

void SettingsDialog::moveDownButtonClick()
{
    if (orderedDictsList->currentRow() < orderedDictsList->count() - 1)
        orderedDictsList->insertItem(orderedDictsList->currentRow(),
                                     orderedDictsList->takeItem(orderedDictsList->currentRow() + 1));
}

void SettingsDialog::moveUpDictDirButtonClick()
{
    if (dictDirsList->currentRow() > 0)
    {
        dictDirsList->insertItem(dictDirsList->currentRow() - 1,
                                 dictDirsList->takeItem(dictDirsList->currentRow()));
        dictDirsList->setCurrentRow(dictDirsList->currentRow() - 1);
    }
}

void SettingsDialog::moveDownDictDirButtonClick()
{
    if (dictDirsList->currentRow() < dictDirsList->count() - 1)
        dictDirsList->insertItem(dictDirsList->currentRow(),
                                 dictDirsList->takeItem(dictDirsList->currentRow() + 1));
}

void SettingsDialog::moveLeftButtonClick()
{
    disabledDictsList->addItem(orderedDictsList->takeItem(orderedDictsList->currentRow()));
}

void SettingsDialog::moveRightButtonClick()
{
    orderedDictsList->addItem(disabledDictsList->takeItem(disabledDictsList->currentRow()));
}

void SettingsDialog::addDictDirButtonClick()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select dictionaries directory"));
    if (! dirName.isEmpty())
    {
        dictDirsList->addItem(dirName);
        updateOrder();
    }
}

void SettingsDialog::removeDictDirButtonClick()
{
    delete dictDirsList->takeItem(dictDirsList->currentRow());
    updateOrder();
}

