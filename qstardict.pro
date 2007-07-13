TEMPLATE = app
TARGET = qstardict
VERSION = 0.01

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

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

RESOURCES += \
    resources/qstardict.qrc
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
    src/settingsdialog.h
SOURCES += \
    src/lib/dictziplib.cpp \
    src/lib/distance.cpp \
    src/lib/lib.cpp \
    src/mainwindow.cpp \
    src/dictcore.cpp \
    src/main.cpp \
    src/popupwindow.cpp \
    src/settingsdialog.cpp

target.path += /usr/local/bin
INSTALLS += target

