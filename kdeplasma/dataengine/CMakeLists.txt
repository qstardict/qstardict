project(plasma-qstardictengine)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../../plugins)


set(qstardict_engine_SRCS
    dictengine.cpp
)

kde4_add_plugin(plasma_engine_qstardict ${qstardict_engine_SRCS})
target_link_libraries(plasma_engine_qstardict
${KDE4_KDECORE_LIBS}
${KDE4_PLASMA_LIBS}
)

set(QSTARDICT_PLUGINS_DIR ${LIB_INSTALL_DIR}/qstardict/plugins)

configure_file(config.h.cmake config.h)


install(TARGETS plasma_engine_qstardict DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-qstardict.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

