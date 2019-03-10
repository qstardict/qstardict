isEmpty(ENABLED_PLUGINS) {
    ENABLED_PLUGINS=stardict multitran anki web print savetofile
    !win32 {
        load(configure)
        qtCompileTest(kde):ENABLED_PLUGINS += kdeintegration
    }
}
