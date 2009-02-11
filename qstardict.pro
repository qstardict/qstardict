#############################################################################
# qstardict.pro - QStarDict, a StarDict clone written with using Qt         #
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

TEMPLATE = subdirs
include(qstardict.pri)
SUBDIRS = qstardict plugins

DOCS += \
    AUTHORS \
    COPYING \
    ChangeLog \
    THANKS
DISTFILES += $$DOCS \
    INSTALL \
    README

message("Enabled plugins: "$$ENABLED_PLUGINS)
unix {
    isEmpty(NO_DBUS) {
        message("D-Bus support: enabled")
    } else {
        message("D-Bus support: disabled")
    }
}
isEmpty(NO_TRANSLATIONS) {
    message("Translations: enabled")
} else {
    message("Translations: disabled")
}
message("Install prefix: "$$INSTALL_PREFIX)
message("Binary directory: "$$BIN_DIR)
message("Data directory: "$$DATA_DIR)
isEmpty(NO_TRANSLATIONS):message("Translations directory: "$$TRANSLATIONS_DIR)
message("Plugins directory: "$$PLUGINS_DIR)
message("Documentation directory: "$$DOCS_DIR)

win32 {
    isEmpty(GLIB2PATH) {
        error("No Glib2 library path (GLIB2PATH) is set.")
	} else {
        message("Glib2 location is set to $$GLIB2PATH")
	}
    isEmpty(ZLIBPATH) {
        error("No zlib library path (ZLIBPATH) is set.")
	} else {
        message("Zlib location is set to $$ZLIBPATH")
	}
}

docs.files = $$DOCS
docs.path = $$DOCS_DIR
INSTALLS += docs

