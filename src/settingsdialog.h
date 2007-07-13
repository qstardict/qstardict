#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ui_settingsdialog.h"

class MainWindow;

class SettingsDialog: public QDialog, private Ui::SettingsDialog
{
        Q_OBJECT

    public:
        SettingsDialog(MainWindow *parent);

    private slots:
        void apply();

        void moveUpButtonClick();
        void moveDownButtonClick();
        void moveLeftButtonClick();
        void moveRightButtonClick();
        void addDictDirButtonClick();
        void removeDictDirButtonClick();
        void moveUpDictDirButtonClick();
        void moveDownDictDirButtonClick();

    private:
        void updateOrder();

        MainWindow *mainWindow;
};

#endif // SETTINGSDIALOG_H

