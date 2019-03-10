#############################################################################
# multitran.pro - QStarDict, a StarDict clone written using Qt              #
# Copyright (C) 2008 Nick Shaforostoff                                      #
# Copyright (C) 2019 Alexander Rodin                                        #
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

TARGET = multitran
META_VERSION = 1.0
META_NAME = Multitran
META_DESC = The Multitran plugin
META_AUTHORS = Andrey Pominov <pom@aha.ru>;Stanislav Ievlev <inger@altlinux.org;Nick Shaforostoff <shaforostoff@kde.ru>;Alexander Rodin <rodin.alexander@gmail.com>
META_FEATURES = dict;defenable

include(../plugin.pri)
win32:CONFIG += plugin

DEFINES += NDEBUG

INCLUDEPATH += libbtree/include/btree \
               libfacet/include/facet \
               libmtquery/include/mt/query \
               libmtsupport/include/mt/support

HEADERS += multitran.h \
           libbtree/include/btree/bdata.hh \
           libbtree/include/btree/bgreater.hh \
           libbtree/include/btree/biterator.hh \
           libbtree/include/btree/bmap.hh \
           libbtree/include/btree/brecset.hh \
           libbtree/include/btree/btraits.hh \
           libbtree/include/btree/btree.hh \
           libbtree/include/btree/btree_dll.hh \
           libfacet/include/facet/alphabet.hh \
           libfacet/include/facet/facet_dll.hh \
           libfacet/include/facet/identification.hh \
           libfacet/include/facet/typographic.hh \
           libmtquery/include/mt/query/article.hh \
           libmtquery/include/mt/query/config.hh \
           libmtquery/include/mt/query/dict_path.hh \
           libmtquery/include/mt/query/file_map.hh \
           libmtquery/include/mt/query/lang_info.hh \
           libmtquery/include/mt/query/lemm.hh \
           libmtquery/include/mt/query/lgk.hh \
           libmtquery/include/mt/query/linguas.hh \
           libmtquery/include/mt/query/mtquery_dll.hh \
           libmtquery/include/mt/query/siktable.hh \
           libmtquery/include/mt/query/singleton.hh \
           libmtquery/include/mt/query/translation.hh \
           libmtsupport/include/mt/support/mtsupport_dll.hh \
           libmtsupport/include/mt/support/path.hh \
           libmtsupport/include/mt/support/shared_ptr.hh \
           libmtsupport/include/mt/support/str.hh
SOURCES += multitran.cpp \
           libbtree/src/biterator.cc \
           libbtree/src/brecset.cc \
           libbtree/src/btree.cc \
           libfacet/src/alphabet.cc \
           libfacet/src/identification.cc \
           libfacet/src/typographic.cc \
           libmtquery/src/article.cc \
           libmtquery/src/config.cc \
           libmtquery/src/dict_path.cc \
           libmtquery/src/file_map.cc \
           libmtquery/src/lang_info.cc \
           libmtquery/src/lemm.cc \
           libmtquery/src/lgk.cc \
           libmtquery/src/linguas.cc \
           libmtquery/src/siktable.cc \
           libmtquery/src/translation.cc \
           libmtsupport/src/path.cc \
           libmtsupport/src/str.cc
