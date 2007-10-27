# QStarDict - Qt version of StarDict

TEMPLATE = app
TARGET = qstardict
VERSION = 0.08-svn
DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"

unix:isEmpty(NO_DBUS):!contains(QT_CONFIG, qdbus): NO_DBUS = 1

QT = \
    gui \
    core
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

FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui
HEADERS += \
    qstardict/lib/dictziplib.hpp \
    qstardict/lib/distance.h \
    qstardict/lib/file.hpp \
    qstardict/lib/lib.h \
    qstardict/lib/mapfile.hpp \
    qstardict/mainwindow.h \
    qstardict/dictcore.h \
    qstardict/popupwindow.h \
    qstardict/settingsdialog.h \
    qstardict/keyboard.h \
    qstardict/dictwidget.h \
    qstardict/resizablepopup.h \
    qstardict/selection.h
unix:isEmpty(NO_DBUS):HEADERS += qstardict/dbusadaptor.h
SOURCES += \
    qstardict/lib/dictziplib.cpp \
    qstardict/lib/distance.cpp \
    qstardict/lib/lib.cpp \
    qstardict/mainwindow.cpp \
    qstardict/dictcore.cpp \
    qstardict/main.cpp \
    qstardict/popupwindow.cpp \
    qstardict/settingsdialog.cpp \
    qstardict/keyboard.cpp \
    qstardict/dictwidget.cpp \
    qstardict/resizablepopup.cpp \
    qstardict/selection.cpp
unix:isEmpty(NO_DBUS):SOURCES += qstardict/dbusadaptor.cpp
RESOURCES += \
    resources/qstardict.qrc
isEmpty(NO_TRANSLATIONS) {
    TRANSLATIONS += \
        translations/qstardict-bg_BG.ts \
        translations/qstardict-cs_CZ.ts \
        translations/qstardict-pt_BR.ts \
        translations/qstardict-ru_RU.ts \
        translations/qstardict-ua_UA.ts \
        translations/qstardict-zh_CN.ts \
        translations/qstardict-zh_TW.ts
    RESOURCES += translations/translations.qrc
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
    INSTALLS += target icons desktop_files
}

! unix: warning("Popup window will not properly work on this platform")

