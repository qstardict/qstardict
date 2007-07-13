#include "settingsdialog.h"

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
                modifierName = "Meta";
                break;
        }
        modifierKeyBox->setCurrentIndex(modifierKeyBox->findText(modifierName));
    }

    connect(moveUpButton, SIGNAL(clicked()), SLOT(moveUpButtonClick()));
    connect(moveDownButton, SIGNAL(clicked()), SLOT(moveDownButtonClick()));
    connect(moveLeftButton, SIGNAL(clicked()), SLOT(moveLeftButtonClick()));
    connect(moveRightButton, SIGNAL(clicked()), SLOT(moveRightButtonClick()));

    connect(this, SIGNAL(accepted()), SLOT(apply()));

    // keyboard modifiers are not done now
    useScanModifierBox->hide();
    modifierKeyBox->hide();
}

void SettingsDialog::apply()
{
    QStringList ordered;

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
        else if (modifierKeyBox->currentText() ==  "Alt")
            modifierKey = Qt::AltModifier;
        else if (modifierKeyBox->currentText() == "Meta")
            modifierKey = Qt::MetaModifier;
    mainWindow->popup->setModifierKey(modifierKey);
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

void SettingsDialog::moveLeftButtonClick()
{
    disabledDictsList->addItem(orderedDictsList->takeItem(orderedDictsList->currentRow()));
}

void SettingsDialog::moveRightButtonClick()
{
    orderedDictsList->addItem(disabledDictsList->takeItem(disabledDictsList->currentRow()));
}

