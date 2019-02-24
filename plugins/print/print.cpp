/*****************************************************************************
 * print.cpp - QStarDict, a StarDict clone written with using Qt             *
 * Copyright (C) 2019 Alexander Rodin                                        *
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

#include "print.h"
#include <QPrinter>
#include <QPrintDialog>

QIcon Print::pluginIcon() const
{
    return QIcon(":/icons/document-print.png");
}

Print::Print(QObject *parent)
    : QObject(parent)
{
}

QIcon Print::toolbarIcon() const {
    return QIcon(":/icons/document-print.png");
}

QString Print::toolbarText() const {
    return tr("Prin&t translation");
}

void Print::execute(QStarDict::DictWidget *dictWidget) {
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, dictWidget);
    if (dialog.exec() == QDialog::Accepted)
        dictWidget->translationView()->print(&printer);
}
