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
macx {
    TARGET = QStarDict
}
include(../qstardict.pri)

FORMS += \
    cssedit.ui \
    mainwindow.ui \
    settingsdialog.ui \
    dictbrowsersearch.ui
HEADERS += \
    application.h \
    cssedit.h \
    dictbrowser.h \
    dictcore.h \
    ../plugins/dictplugin.h \
    ../plugins/baseplugin.h \
    ../plugins/trayplugin.h \
    ../plugins/pluginserver.h \
    ../plugins/metadata.h \
    dictwidget.h \
    keyboard.h \
    mainwindow.h \
    popupwindow.h \
    resizablepopup.h \
    selection.h \
    settingsdialog.h \
    speaker.h \
    trayicon.h \
    dictbrowsersearch.h \
    pluginsview.h \
    pluginsmodel.h \
    pluginmanager.h \
    appinfo.h \
    util.h

unix:isEmpty(NO_DBUS):HEADERS += \
    dbusadaptor.h
SOURCES += \
    application.cpp \
    cssedit.cpp \
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
    speaker.cpp \
    trayicon.cpp \
    dictbrowsersearch.cpp \
    pluginsview.cpp \
    pluginsmodel.cpp \
    pluginmanager.cpp \
    appinfo.cpp \
    util.cpp

unix:isEmpty(NO_DBUS):SOURCES += \
    dbusadaptor.cpp

# From https://github.com/hluk/CopyQ/tree/92ec805b41bd097cbe523c08e320e3d32fc1c511
unix | win32 {
    DEFINES += BUILD_QXT_GUI

    QXT_DIR = ..
    INCLUDEPATH += $$QXT_DIR

    HEADERS += $$QXT_DIR/qxt/qxtglobal.h
    SOURCES += $$QXT_DIR/qxt/qxtglobal.cpp

    HEADERS += $$QXT_DIR/qxt/qxtglobalshortcut.h
    HEADERS += $$QXT_DIR/qxt/qxtglobalshortcut_p.h
    SOURCES += $$QXT_DIR/qxt/qxtglobalshortcut.cpp

    unix:!macx: SOURCES += $$QXT_DIR/qxt/qxtglobalshortcut_x11.cpp
    macx: SOURCES += $$QXT_DIR/qxt/qxtglobalshortcut_mac.cpp
    win32: SOURCES += $$QXT_DIR/qxt/qxtglobalshortcut_win.cpp

    QT += gui-private
}

RESOURCES += \
    pixmaps/pixmaps.qrc
win32:RC_FILE += \
    qstardict.rc
DISTFILES += \
    qstardict.png \
    qstardict.desktop

target.path = $$BIN_DIR
INSTALLS += target

unix:!macx {
    desktop_icon.files = qstardict.png
    desktop_icon.path = $$INSTALL_PREFIX/share/pixmaps
    desktop_icon.files = qstardict.svg
    desktop_icon.path = $$INSTALL_PREFIX/share/icons/hicolor/scalable/apps
    desktop_file.files = qstardict.desktop
    desktop_file.path = $$INSTALL_PREFIX/share/applications
    INSTALLS += desktop_icon desktop_file
}
macx {
    ICON = pixmaps/qstardict.icns
}
