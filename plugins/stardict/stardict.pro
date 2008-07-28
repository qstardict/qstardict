#############################################################################
# stardict.pro - QStarDict, a StarDict clone written with using Qt          #
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

TARGET = stardict
include(../plugin.pri)
win32:CONFIG += plugin

HEADERS += \
    stardict.h \
    dictziplib.hpp \
    distance.h \
    file.hpp \
    lib.h \
    mapfile.hpp \
    settingsdialog.h
SOURCES += \
    stardict.cpp \
    dictziplib.cpp \
    distance.cpp \
    lib.cpp \
    settingsdialog.cpp
FORMS += \
    settingsdialog.ui
RESOURCES += \
    pixmaps/pixmaps.qrc

