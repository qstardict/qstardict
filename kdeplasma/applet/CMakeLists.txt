project(plasma-qstardict)

set(qstardict_SRCS
    dict.cpp)

kde4_add_plugin(plasma_applet_qstardict ${qstardict_SRCS})
target_link_libraries(plasma_applet_qstardict ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
)

install(TARGETS plasma_applet_qstardict DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-qstardict-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
