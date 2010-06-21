#############################################################################
# translations.pri - QStarDict, a StarDict clone written with using Qt      #
# Copyright (C) 2008-2009 Alexander Rodin                                   #
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
    qstardict-bg_BG.ts \
    qstardict-cs_CZ.ts \
    qstardict-de_DE.ts \
    qstardict-es_ES.ts \
    qstardict-fr_FR.ts \
    qstardict-it_IT.ts \
    qstardict-lt_LT.ts \
    qstardict-pl_PL.ts \
    qstardict-pt_BR.ts \
    qstardict-ru_RU.ts \
    qstardict-ua_UA.ts \
    qstardict-tr_TR.ts \
    qstardict-zh_CN.ts \
    qstardict-zh_TW.ts

COMPILED_TRANSLATIONS += \
    qstardict-bg_BG.qm \
    qstardict-cs_CZ.qm \
    qstardict-de_DE.qm \
    qstardict-es_ES.qm \
    qstardict-fr_FR.qm \
    qstardict-it_IT.qm \
    qstardict-lt_LT.qm \
    qstardict-pl_PL.qm \
    qstardict-pt_BR.qm \
    qstardict-ru_RU.qm \
    qstardict-ua_UA.qm \
    qstardict-tr_TR.qm \
    qstardict-zh_CN.qm \
    qstardict-zh_TW.qm
DISTFILES += $$COMPILED_TRANSLATIONS

isEmpty(NO_TRANSLATIONS) {
    for(t, COMPILED_TRANSLATIONS):translations.files += "translations/$${t}"
    translations.path = $$TRANSLATIONS_DIR
    INSTALLS += translations
}
