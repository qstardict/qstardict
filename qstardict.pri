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

VERSION = 0.13.1

QT = \
    core \
    gui \
    network \
    xml
CONFIG += \
    qt \
    warn_on \
    release
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0
    LIBS += -lz
}
macx {
    # universal binaries
    CONFIG += x86 #x86_64 # not available on Snow Leopard ppc ppc64
#    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk/
}
win32 {
    CONFIG += console
    LIBS += -lzdll -lglib-2.0 -luser32
    INCLUDEPATH += $$GLIB2PATH/include/glib-2.0 \
                   $$GLIB2PATH/lib/glib-2.0/include \
                   $$ZLIBPATH/include
    QMAKE_LIBDIR += $$GLIB2PATH/lib \
                    $$ZLIBPATH/lib
}

unix:DEFINES += HAVE_MMAP
unix:isEmpty(NO_DBUS):!contains(QT_CONFIG, dbus): NO_DBUS = 1
unix:isEmpty(NO_DBUS):CONFIG += qdbus
unix:isEmpty(NO_DBUS):DEFINES += QSTARDICT_WITH_DBUS
isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_WITH_TRANSLATIONS

isEmpty(ENABLED_PLUGINS):ENABLED_PLUGINS="stardict web"

unix {
    macx {
        isEmpty(INSTALL_PREFIX):INSTALL_PREFIX=/opt
        # helper var to save text duplicity.
        # NOTE: it cannot use $$TARGET because the target is
        # different in every directory. Obviously.
        MAC_BUNDLE_PATH=$$INSTALL_PREFIX/QStarDict.app/Contents
        # bin dir is used for qstardict dir/main application
        # qmake created bundle for it itself. So it *has* to be
        # the same as install location
        BIN_DIR=$$INSTALL_PREFIX
        DATA_DIR=$$MAC_BUNDLE_PATH/share
        TRANSLATIONS_DIR=$$MAC_BUNDLE_PATH/i18n
        PLUGINS_DIR=$$MAC_BUNDLE_PATH/lib
        DOCS_DIR=$$MAC_BUNDLE_PATH/share/doc

        DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
    }
    else {
        isEmpty(INSTALL_PREFIX):INSTALL_PREFIX=/usr
        isEmpty(BIN_DIR):BIN_DIR=$$INSTALL_PREFIX/bin
        isEmpty(DATA_DIR):DATA_DIR=$$INSTALL_PREFIX/share/qstardict
        isEmpty(NO_TRANSLATIONS):isEmpty(TRANSLATIONS_DIR):TRANSLATIONS_DIR=$$DATA_DIR/translations
        isEmpty(PLUGINS_DIR):PLUGINS_DIR=$$INSTALL_PREFIX/lib/qstardict/plugins
        isEmpty(DOCS_DIR):DOCS_DIR=$$INSTALL_PREFIX/share/doc/qstardict

        DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
        DEFINES += QSTARDICT_INSTALL_PREFIX=\\\"$$INSTALL_PREFIX\\\"
        DEFINES += QSTARDICT_BIN_DIR=\\\"$$BIN_DIR\\\"
        DEFINES += QSTARDICT_DATA_DIR=\\\"$$DATA_DIR\\\"
        isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\\\"$$TRANSLATIONS_DIR\\\"
        DEFINES += QSTARDICT_PLUGINS_DIR=\\\"$$PLUGINS_DIR\\\"
    }
} else:win32 {
     isEmpty(INSTALL_PREFIX) {
         INSTALL_PREFIX=
     } else {
        # handle path backslashes in defines correctly
         INSTALL_PREFIX = $$re_escape($$INSTALL_PREFIX)
    }
    isEmpty(BIN_DIR):BIN_DIR=$$INSTALL_PREFIX/
    isEmpty(DATA_DIR):DATA_DIR=$$INSTALL_PREFIX/data
    isEmpty(NO_TRANSLATIONS):isEmpty(TRANSLATIONS_DIR):TRANSLATIONS_DIR=$$INSTALL_PREFIX/translations
    isEmpty(PLUGINS_DIR):PLUGINS_DIR=$$INSTALL_PREFIX/plugins
    isEmpty(DOCS_DIR):DOCS_DIR=$$INSTALL_PREFIX/docs

    DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
#    DEFINES += QSTARDICT_INSTALL_PREFIX=\(QApplication::applicationDirPath\(\)+\\\"$$INSTALL_PREFIX\\\"\)
#    DEFINES += QSTARDICT_BIN_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$BIN_DIR\\\"\)
#    DEFINES += QSTARDICT_DATA_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$DATA_DIR\\\"\)
#    isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$TRANSLATIONS_DIR\\\"\)
#    DEFINES += QSTARDICT_PLUGINS_DIR=\(QApplication::applicationDirPath\(\)+\\\"$$PLUGINS_DIR\\\"\)
    DEFINES += QSTARDICT_INSTALL_PREFIX=\\\"$$INSTALL_PREFIX\\\"
    DEFINES += QSTARDICT_BIN_DIR=\\\"$$BIN_DIR\\\"
    DEFINES += QSTARDICT_DATA_DIR=\\\"$$DATA_DIR\\\"
    isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\\\"$$TRANSLATIONS_DIR\\\"
    DEFINES += QSTARDICT_PLUGINS_DIR=\\\"$$PLUGINS_DIR\\\"
}

system(echo "ENABLED_PLUGINS = $$ENABLED_PLUGINS" >plugins/plugins.pri)
