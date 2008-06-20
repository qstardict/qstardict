#############################################################################
# translations.pri - QStarDict, a StarDict clone written with using Qt      #
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

TRANSLATIONS += \
    translations/qstardict-bg_BG.ts \
    translations/qstardict-cs_CZ.ts \
    translations/qstardict-de_DE.ts \
    translations/qstardict-fr_FR.ts \
    translations/qstardict-it_IT.ts \
    translations/qstardict-pl_PL.ts \
    translations/qstardict-pt_BR.ts \
    translations/qstardict-ru_RU.ts \
    translations/qstardict-ua_UA.ts \
    translations/qstardict-zh_CN.ts \
    translations/qstardict-zh_TW.ts

COMPILED_TRANSLATIONS += \
    translations/qstardict-bg_BG.qm \
    translations/qstardict-cs_CZ.qm \
    translations/qstardict-de_DE.qm \
    translations/qstardict-fr_FR.qm \
    translations/qstardict-it_IT.qm \
    translations/qstardict-pl_PL.qm \
    translations/qstardict-pt_BR.qm \
    translations/qstardict-ru_RU.qm \
    translations/qstardict-ua_UA.qm \
    translations/qstardict-zh_CN.qm \
    translations/qstardict-zh_TW.qm
DISTFILES += $$COMPILED_TRANSLATIONS

isEmpty(NO_TRANSLATIONS) {
    translations.files = $$COMPILED_TRANSLATIONS
    translations.path = $$TRANSLATIONS_DIR
    INSTALLS += translations
}
