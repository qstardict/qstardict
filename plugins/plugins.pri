isEmpty(ENABLED_PLUGINS) {
    ENABLED_PLUGINS=stardict anki web
    !win32 {
        greaterThan(QT_MAJOR_VERSION, 4) {
            load(configure)
            qtCompileTest(kde):ENABLED_PLUGINS += kdeintegration
        } else {
            exists(/usr/include/KDE):ENABLED_PLUGINS += kdeintegration
        }
    }
}
