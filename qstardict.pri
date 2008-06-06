#############################################################################
# qstardict.pri - QStarDict, a StarDict clone written with using Qt         #
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

VERSION = 0.12.7.9999

QT = \
    core \
    gui \
    network \
    xml
CONFIG += \
    qt \
    warn_on \
    release
CONFIG += \
    link_pkgconfig
    PKGCONFIG += \
    glib-2.0
unix:LIBS += -lz
windows:LIBS += -lzdll

unix:DEFINES += HAVE_MMAP
unix:isEmpty(NO_DBUS):!contains(QT_CONFIG, qdbus): NO_DBUS = 1
unix:isEmpty(NO_DBUS):CONFIG += qdbus
unix:isEmpty(NO_DBUS):DEFINES += QSTARDICT_WITH_DBUS
isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_WITH_TRANSLATIONS

unix {
    isEmpty(INSTALL_PREFIX):INSTALL_PREFIX=/usr
    isEmpty(BIN_DIR):BIN_DIR=$$INSTALL_PREFIX/bin
    isEmpty(DATA_DIR):DATA_DIR=$$INSTALL_PREFIX/share/qstardict
    isEmpty(NO_TRANSLATIONS):isEmpty(TRANSLATIONS_DIR):TRANSLATIONS_DIR=$$DATA_DIR/translations
    isEmpty(PLUGINS_DIR):PLUGINS_DIR=$$INSTALL_PREFIX/lib/qstardict/plugins

    DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
    DEFINES += QSTARDICT_INSTALL_PREFIX=\\\"$$INSTALL_PREFIX\\\"
    DEFINES += QSTARDICT_BIN_DIR=\\\"$$BIN_DIR\\\"
    DEFINES += QSTARDICT_DATA_DIR=\\\"$$DATA_DIR\\\"
    isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\\\"$$TRANSLATIONS_DIR\\\"
    DEFINES += QSTARDICT_PLUGINS_DIR=\\\"$$PLUGINS_DIR\\\"
} else:win32 {
    isEmpty(INSTALL_PREFIX):INSTALL_PREFIX=/
    isEmpty(BIN_DIR):BIN_DIR=$$INSTALL_PREFIX
    isEmpty(DATA_DIR):DATA_DIR=$$INSTALL_PREFIX"data"
    isEmpty(NO_TRANSLATIONS):isEmpty(TRANSLATIONS_DIR):TRANSLATIONS_DIR=$$INSTALL_PREFIX"translations"
    isEmpty(PLUGINS_DIR):PLUGINS_DIR=$$INSTALL_PREFIX"plugins"

    DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
    DEFINES += QSTARDICT_INSTALL_PREFIX=\(QApplication::applicationDirPath\(\)+\\\"$$INSTALL_PREFIX\\\"\)
    DEFINES += QSTARDICT_BIN_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$BIN_DIR\\\"\)
    DEFINES += QSTARDICT_DATA_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$DATA_DIR\\\"\)
    isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$TRANSLATIONS_DIR\\\"\)
    DEFINES += QSTARDICT_PLUGINS_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$PLUGINS_DIR\\\"\)
}
