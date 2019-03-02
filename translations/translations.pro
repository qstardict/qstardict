#############################################################################
# translations.pro - QStarDict, a StarDict clone written with using Qt      #
# Copyright (C) 2008-2019 Alexander Rodin                                   #
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

TEMPLATE = aux

include(../qstardict.pri)
include(translations.pri)

# from https://github.com/Arora/arora/blob/e310d632e9f6c135c376576d2d466af03fd219ee/src/locale/locale.pri
isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt4 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

# from https://github.com/Arora/arora/blob/e310d632e9f6c135c376576d2d466af03fd219ee/src/locale/locale.pri
updateqm.input = TRANSLATIONS
updateqm.output = $$TRANSLATIONS_SRC_DIR/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm $$TRANSLATIONS_SRC_DIR/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

isEmpty(NO_TRANSLATIONS) {
    translations.CONFIG += no_check_exist

    for(t, TRANSLATIONS) {
        t ~= "s/^(.+).ts$/\\1.qm"
        translations.files += $$t
        DISTFILES          += $$t # For what?
    }

    translations.path = $$TRANSLATIONS_DIR
    INSTALLS += translations
}
