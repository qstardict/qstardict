/*****************************************************************************
 * main.cpp - QStarDict, a StarDict clone written with using Qt              *
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

#include "application.h"

#ifdef Q_OS_UNIX
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <unistd.h>
#elif defined(Q_OS_WIN) // Q_OS_UNIX
#include <windows.h>
#include <QMessageBox>
#endif // Q_OS_WIN

#ifdef QSTARDICT_WITH_TRANSLATIONS
#include <QLocale>
#include <QTranslator>
#endif // QSTARDICT_WITH_TRANSLATIONS

int main(int argc, char *argv[])
{
    QStarDict::Application app(argc, argv);
#ifdef Q_OS_UNIX
    QSettings lockFile(QDir::homePath() + "/.qstardict/qstardict.pid", QSettings::IniFormat);
    QString lastPid = lockFile.value("LastStart/pid").toString();
    if (lastPid.length() && QDir("/proc/" + lastPid).exists() &&
        lockFile.value("LastStart/time").toDateTime() == QFileInfo("/proc/" + lastPid).created())
    {
        qDebug("qstardict: already running");
        return 0;
    }
    lockFile.setValue("LastStart/pid", getpid());
    lockFile.setValue("LastStart/time", QFileInfo("/proc/" + QString::number(getpid())).created());
    lockFile.sync();
#elif defined(Q_OS_WIN) // Q_OS_UNIX
    HANDLE hMutex = CreateMutex(NULL, true, (LPCTSTR)"qstardict");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::information(0, "Warning", "QStarDict is already running");
        // Strange encoding issue...
        // MessageBox(0, (LPCWSTR)"Warning", (LPCWSTR)"QStarDict is already running", MB_ICONWARNING);
        return 0;
    }
#endif // Q_OS_WIN


    return app.exec();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

