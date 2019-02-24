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

# It's common for all subprjects file. Keep it in mind changing it

VERSION = 1.3

QT = \
    core \
    gui \
    network \
    xml

QT += widgets printsupport

CONFIG += \
    qt \
    warn_on

CONFIG += c++11

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0
    macx {
        LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
    } else {
        LIBS += -lX11
    }
    LIBS += -lz
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

!isEmpty(DEVEL):DEFINES += DEVEL
unix:DEFINES += HAVE_MMAP
unix:isEmpty(NO_DBUS):!contains(QT_CONFIG, dbus): NO_DBUS = 1
unix:isEmpty(NO_DBUS):QT += dbus
unix:isEmpty(NO_DBUS):DEFINES += QSTARDICT_WITH_DBUS
isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_WITH_TRANSLATIONS
include(plugins/plugins.pri)

unix {
    macx {
        isEmpty(INSTALL_PREFIX) {
             INSTALL_PREFIX=/Applications
        }
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
        MAN_DIR=$$MAC_BUNDLE_PATH/share/man/man1

        bundle.target = bundle
        bundle.depends = install
        bundle.commands = "mkdir -p bundle && cp -R $$INSTALL_PREFIX/QStarDict.app bundle/QStarDict.app && ln -s /Applications bundle/Applications"

        dmg.target = "QStarDict.dmg"
        dmg.depends = bundle
        dmg.commands = "rm -f QStarDict.dmg QStarDict_tmp.dmg && hdiutil create QStarDict_tmp.dmg -ov -volname QStarDict -fs HFS+ -srcfolder bundle && hdiutil convert QStarDict_tmp.dmg -format UDZO -o QStarDict.dmg"

        QMAKE_EXTRA_TARGETS += bundle dmg
    }
    else {
        isEmpty(INSTALL_PREFIX):INSTALL_PREFIX=/usr
        isEmpty(BIN_DIR):BIN_DIR=$$INSTALL_PREFIX/bin
        isEmpty(DATA_DIR):DATA_DIR=$$INSTALL_PREFIX/share/qstardict
        isEmpty(NO_TRANSLATIONS):isEmpty(TRANSLATIONS_DIR):TRANSLATIONS_DIR=$$DATA_DIR/translations
        isEmpty(LIB_DIR):LIB_DIR=$$INSTALL_PREFIX/lib/qstardict
        isEmpty(PLUGINS_DIR):PLUGINS_DIR=$$LIB_DIR/plugins
        isEmpty(DOCS_DIR):DOCS_DIR=$$INSTALL_PREFIX/share/doc/qstardict
        isEmpty(MAN_DIR):MAN_DIR=$$INSTALL_PREFIX/share/man/man1
    }
    DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
    DEFINES += QSTARDICT_INSTALL_PREFIX=\\\"$$INSTALL_PREFIX\\\"
    DEFINES += QSTARDICT_BIN_DIR=\\\"$$BIN_DIR\\\"
    DEFINES += QSTARDICT_DATA_DIR=\\\"$$DATA_DIR\\\"
    isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\\\"$$TRANSLATIONS_DIR\\\"
    DEFINES += QSTARDICT_PLUGINS_DIR=\\\"$$PLUGINS_DIR\\\"

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
    isEmpty(MAN_DIR):MAN_DIR=$$INSTALL_PREFIX/man

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
