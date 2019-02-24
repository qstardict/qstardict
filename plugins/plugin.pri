#############################################################################
# plugin.pri - QStarDict, a StarDict clone written using Qt                 #
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

isEmpty(TARGET):error(You must set TARGET before including $$_FILE_)
isEmpty(META_DESC):error(You must set META_DESC before including $$_FILE_)

TEMPLATE = lib
QMAKE_TARGET_PRODUCT = QStarDict
QMAKE_TARGET_DESCRIPTION = $$META_DESC

include(../qstardict.pri)

VERSION = $$META_VERSION

CONFIG += plugin
INCLUDEPATH += $$dirname(PWD)

target.path = $$PLUGINS_DIR
INSTALLS += target

TARGET_JSON = "\
{ \
    \"id\": \"$$TARGET\", \
    \"name\": \"$$META_NAME\", \
    \"version\" : \"$$VERSION\", \
    \"description\": \"$$META_DESC\", \
    \"authors\": \"$$META_AUTHORS\", \
    \"features\": \"$$META_FEATURES\" \
}"
write_file($${TARGET}/$${TARGET}.json, TARGET_JSON)

OTHER_FILES += $${TARGET}.json
# the same as in json
DEFINES += PLUGIN_ID=\\\"$$TARGET\\\"
