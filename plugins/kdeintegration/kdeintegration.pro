TARGET = kdeintegration
META_VERSION = 0.1
META_NAME = KDE Integration
META_DESC = Makes QStarDict fill more native on KDE
META_AUTHORS = Sergey Il\'inykh <rion4ik@gmail.com>
META_FEATURES = de;defenable

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


