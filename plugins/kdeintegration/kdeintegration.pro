TARGET = kdeintegration
include(../plugin.pri)

QT *= gui
greaterThan(QT_MAJOR_VERSION, 4) {
	QT *= widgets KGlobalAccel KWindowSystem KNotifications
	DEFINES += USE_KDE5
} else {
	INCLUDEPATH += /usr/include/KDE
	LIBS += -lkdeui
}

SOURCES += \
	kdeintegration.cpp

HEADERS += \
	kdeintegration.h

RESOURCES += \
    kdeintegration.qrc

OTHER_FILES += \
    kdeintegration.json


