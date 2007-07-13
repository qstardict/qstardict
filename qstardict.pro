# QStarDict - Qt version of StarDict

TEMPLATE = app
TARGET = qstardict
VERSION = 0.03
DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"

QT = \
    gui \
    core
CONFIG += \
    qt \
    link_pkgconfig \
    warn_on \
    release 
PKGCONFIG += \
    glib-2.0
unix:DEFINES += HAVE_MMAP

FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui
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
    src/keyboard.h
SOURCES += \
    src/lib/dictziplib.cpp \
    src/lib/distance.cpp \
    src/lib/lib.cpp \
    src/mainwindow.cpp \
    src/dictcore.cpp \
    src/main.cpp \
    src/popupwindow.cpp \
    src/settingsdialog.cpp \
    src/keyboard.cpp
RESOURCES += \
    resources/qstardict.qrc \
    translations/translations.qrc
TRANSLATIONS += \
    translations/qstardict-ru_RU.ts \
    translations/qstardict-zh_TW.ts
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
    icon.files = resources/qstardict.png
    icon.path = $$INSTALL_PREFIX/share/pixmaps
    desktop_file.files = resources/qstardict.desktop
    desktop_file.path = $$INSTALL_PREFIX/share/applications
    INSTALLS += target icon desktop_file
}

! unix: warning("Popup window will not properly work on this platform")

