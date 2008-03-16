#############################################################################
# qstardict.pro - QStarDict, a StarDict clone written with using Qt         #
# Copyright (C) 2008 Alexander Rodin                                        #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License along   #
# with this program; if not, write to the Free Software Foundation, Inc.,   #
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               #
#############################################################################

TEMPLATE = app
TARGET = qstardict
include(../qstardict.pri)

FORMS += \
    adddictionarydialog.ui \
    mainwindow.ui \
    settingsdialog.ui
HEADERS += \
    application.h \
    dictbrowser.h \
    dictcore.h \
    dictplugin.h \
    dictwidget.h \
    keyboard.h \
    mainwindow.h \
    popupwindow.h \
    resizablepopup.h \
    selection.h \
    settingsdialog.h \
    trayicon.h
unix:isEmpty(NO_DBUS):HEADERS += dbusadaptor.h
SOURCES += \
    application.cpp \
    dictbrowser.cpp \
    dictcore.cpp \
    dictwidget.cpp \
    keyboard.cpp \
    main.cpp \
    mainwindow.cpp \
    popupwindow.cpp \
    resizablepopup.cpp \
    selection.cpp \
    settingsdialog.cpp \
    trayicon.cpp
unix:isEmpty(NO_DBUS):SOURCES += dbusadaptor.cpp
RESOURCES += \
    pixmaps/pixmaps.qrc
DISTFILES += \
    qstardict.png \
    qstardict.desktop

target.path = $$BIN_DIR
INSTALLS += target

unix {
    desktop_icon.files = qstardict.png
    desktop_icon.path = $$INSTALL_PREFIX/share/pixmaps
    desktop_file.files = qstardict.desktop
    desktop_file.path = $$INSTALL_PREFIX/share/applications
    INSTALLS += desktop_icon desktop_file
}

