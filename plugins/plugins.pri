isEmpty(ENABLED_PLUGINS) {
    ENABLED_PLUGINS=stardict anki web print savetofile
    !win32 {
        load(configure)
        qtCompileTest(kde):ENABLED_PLUGINS += kdeintegration
    }
}
