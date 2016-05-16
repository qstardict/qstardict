isEmpty(ENABLED_PLUGINS) {
	ENABLED_PLUGINS=stardict web

	!win32 {
		greaterThan(QT_MAJOR_VERSION, 4) {
				KDE5INC=/usr/include/KF5
				exists($${KDE5INC}/KNotifications):exists($${KDE5INC}/KWindowSystem):ENABLED_PLUGINS += kdeintegration
		} else {
				exists(/usr/include/KDE):ENABLED_PLUGINS += kdeintegration
		}
	}
}
