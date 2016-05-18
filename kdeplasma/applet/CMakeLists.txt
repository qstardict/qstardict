project(plasma-qstardict)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(qstardict_SRCS
    dict.cpp)

if (KF5)
	find_package(Qt5Widgets REQUIRED)
	find_package(KF5Completion REQUIRED)
	find_package(KF5ConfigWidgets REQUIRED)
    #include_directories(${Qt5Widgets_INCLUDES}Qt5::Widgets)

	set( CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )

    add_library(plasma_applet_qstardict MODULE ${qstardict_SRCS})
    set_target_properties(plasma_applet_qstardict PROPERTIES PREFIX "")
    target_link_libraries(plasma_applet_qstardict Qt5::Widgets Qt5::Gui KF5::Plasma KF5::Completion KF5::ConfigWidgets)
    plasma_install_package(. qstardict)

else (KF5)
	kde4_add_plugin(plasma_applet_qstardict ${qstardict_SRCS})
	target_link_libraries(plasma_applet_qstardict ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
	install(TARGETS plasma_applet_qstardict DESTINATION ${PLUGIN_INSTALL_DIR})
	install(FILES plasma-qstardict-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
endif (KF5)
