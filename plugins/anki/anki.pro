#############################################################################
# anki.pro - QStarDict, a StarDict clone written with using Qt              #
# Copyright (C) 2018 Alexander Rodin                                        #
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

TARGET = anki
META_VERSION = 1.0
META_NAME = Anki
META_DESC = A plugin for Anki
META_AUTHORS = Alexander Rodin <rodin.alexander@gmail.com>
META_FEATURES = toolbar;defenable

include(../plugin.pri)

HEADERS += \
    settingsdialog.h \
    anki.h
SOURCES += \
    settingsdialog.cpp \
    anki.cpp
FORMS += \
    settingsdialog.ui
RESOURCES += \
    pixmaps/pixmaps.qrc
