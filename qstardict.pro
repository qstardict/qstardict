# QStarDict - Qt version of StarDict

TEMPLATE = app
TARGET = qstardict
VERSION = 0.09-svn
DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"

unix:isEmpty(NO_DBUS):!contains(QT_CONFIG, qdbus): NO_DBUS = 1

QT = \
    gui \
    core \
    network \
    xml
CONFIG += \
    qt \
    link_pkgconfig \
    warn_on \
    release
unix:isEmpty(NO_DBUS): CONFIG += qdbus
PKGCONFIG += \
    glib-2.0
unix:DEFINES += HAVE_MMAP
unix:isEmpty(NO_DBUS):DEFINES += QSTARDICT_WITH_DBUS
isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_WITH_TRANSLATIONS
isEmpty(NO_TRANSLATIONS):!isEmpty(SEPARATE_TRANSLATIONS):DEFINES += QSTARDICT_WITH_SEPARATE_TRANSLATIONS

FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui \
    ui/adddictionarydialog.ui
HEADERS += \
    src/lib/dictziplib.hpp \
    src/lib/distance.h \
    src/lib/file.hpp \
    src/lib/lib.h \
    src/lib/mapfile.hpp \
    src/mainwindow.h \
    src/dictcore.h \
    src/popupwindow.h \
    src/settingsdialog.h \
    src/keyboard.h \
    src/dictwidget.h \
    src/resizablepopup.h \
    src/selection.h \
    src/adddictionarydialog.h \
    src/downloadprogressdialog.h \
    src/iso639.h
unix:isEmpty(NO_DBUS):HEADERS += src/dbusadaptor.h
SOURCES += \
    src/lib/dictziplib.cpp \
    src/lib/distance.cpp \
    src/lib/lib.cpp \
    src/mainwindow.cpp \
    src/dictcore.cpp \
    src/main.cpp \
    src/popupwindow.cpp \
    src/settingsdialog.cpp \
    src/keyboard.cpp \
    src/dictwidget.cpp \
    src/resizablepopup.cpp \
    src/selection.cpp \
    src/adddictionarydialog.cpp \
    src/downloadprogressdialog.cpp \
    src/iso639.cpp
unix:isEmpty(NO_DBUS):SOURCES += src/dbusadaptor.cpp
RESOURCES += \
    resources/qstardict.qrc
isEmpty(NO_TRANSLATIONS) {
    TRANSLATIONS += \
        translations/qstardict-bg_BG.ts \
        translations/qstardict-cs_CZ.ts \
        translations/qstardict-pl_PL.ts \
        translations/qstardict-pt_BR.ts \
        translations/qstardict-ru_RU.ts \
        translations/qstardict-ua_UA.ts \
        translations/qstardict-zh_CN.ts \
        translations/qstardict-zh_TW.ts
    isEmpty(SEPARATE_TRANSLATIONS):RESOURCES += translations/translations.qrc
}
DISTFILES += \
    AUTHORS \
    COPYNG \
    ChangeLog \
    INSTALL \
    README \
    resources/qstardict.desktop

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

unix {
    INSTALL_PREFIX=/usr

    target.path = $$INSTALL_PREFIX/bin
    icons.files += resources/qstardict.png
    icons.path = $$INSTALL_PREFIX/share/pixmaps
    desktop_files.files += resources/qstardict.desktop
    desktop_files.path = $$INSTALL_PREFIX/share/applications
    isEmpty(NO_TRANSLATIONS):!isEmpty(SEPARATE_TRANSLATIONS) {
        TRANSLATIONS_DIR = $$INSTALL_PREFIX/share/qstardict/translations
        translations.path = $$TRANSLATIONS_DIR
        translations.files = $$system("grep '<file>' translations/translations.qrc | sed 's/^\s*<file>\s*/translations\//' | sed 's/<\/file>$//'")
        DEFINES += QSTARDICT_TRANSLATIONS_DIR=\\\"$$TRANSLATIONS_DIR\\\"
        INSTALLS += translations
    }
    INSTALLS += target icons desktop_files
} else {
    isEmpty(NO_TRANSLATIONS):!isEmpty(SEPARATE_TRANSLATIONS) {
        DEFINES += QSTARDICT_TRANSLATIONS_DIR="translations/"
    }
}

isEmpty(NO_DBUS) {
    message("D-Bus support is enabled")
} else {
    message("D-Bus support is disabled")
}

isEmpty(NO_TRANSLATIONS) {
    isEmpty(SEPARATE_TRANSLATIONS) {
        message("Built-in translations are enabled")
    } else {
        message("Separate translations are enabled")
    }
} else {
    message("Translations are disabled") 
}
!isEmpty(INSTALL_PREFIX) {
    message("Install prefix is "$$INSTALL_PREFIX)
}
! unix: warning("Popup window will not properly work on this platform")

