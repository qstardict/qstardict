#############################################################################
# web.pro - QStarDict, a StarDict clone written with using Qt               #
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

TARGET = web
VERSION = 0.1
META_NAME = Web
META_DESC = An experimental plugin for WEB dictionaries.<br><b><font color=red>Warning:</font></b> now this plugin is unstable
META_AUTHORS = Alexander Rodin <rodin.alexander@gmail.com>
META_FEATURES = dict

include(../plugin.pri)

HEADERS += \
    settingsdialog.h \
    web.h
SOURCES += \
    settingsdialog.cpp \
    web.cpp
FORMS += \
    adddictionarydialog.ui \
    settingsdialog.ui
RESOURCES += \
    pixmaps/pixmaps.qrc
