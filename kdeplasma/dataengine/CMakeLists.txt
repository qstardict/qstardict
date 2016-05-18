project(plasma-qstardictengine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../../plugins)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(qstardict_engine_SRCS
    dictengine.cpp
)

if (KF5)
	add_library(plasma_engine_qstardict MODULE ${qstardict_engine_SRCS})
	set_target_properties(plasma_engine_qstardict PROPERTIES PREFIX "")
	target_link_libraries(plasma_engine_qstardict KF5::Plasma)

else (KF5)
	kde4_add_plugin(plasma_engine_qstardict ${qstardict_engine_SRCS})
	target_link_libraries(plasma_engine_qstardict
	${KDE4_KDECORE_LIBS}
	${KDE4_PLASMA_LIBS}
	)
endif (KF5)

set(QSTARDICT_PLUGINS_DIR ${LIB_INSTALL_DIR}/qstardict/plugins)

configure_file(config.h.cmake config.h)


install(TARGETS plasma_engine_qstardict DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-qstardict.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

