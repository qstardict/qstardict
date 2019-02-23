isEmpty(ENABLED_PLUGINS) {
    ENABLED_PLUGINS=stardict anki web
    !win32 {
        load(configure)
        qtCompileTest(kde):ENABLED_PLUGINS += kdeintegration
    }
}
