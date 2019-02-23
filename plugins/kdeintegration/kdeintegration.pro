TARGET = kdeintegration
META_VERSION = 0.1
META_NAME = KDE Integration
META_DESC = Makes QStarDict fill more native on KDE
META_AUTHORS = Sergey Il\'inykh <rion4ik@gmail.com>
META_FEATURES = de;defenable

include(../plugin.pri)

QT *= gui widgets KGlobalAccel KWindowSystem KNotifications
DEFINES += USE_KDE5

SOURCES += \
	kdeintegration.cpp

HEADERS += \
	kdeintegration.h

RESOURCES += \
    kdeintegration.qrc

OTHER_FILES += \
    kdeintegration.json


