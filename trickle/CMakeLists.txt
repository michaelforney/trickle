set(TrickleSources
    main.cpp
	filemodel.cpp
	directoryitem.cpp
	filemodelitem.cpp
	trackerinfo.cpp
	trackermodel.cpp
	trackeritem.cpp
	trickle.cpp
	log.cpp
	torrentwidget.cpp
	torrentmodel.cpp
	torrentview.cpp
	torrentinfo.cpp
	fileinfo.cpp
	fileview.cpp
	servermodel.cpp
	torrentsortmodel.cpp
	serverinfo.cpp
	serversettings.cpp
	generalsettings.cpp
	configdialog.cpp
	servereditdialog.cpp
	serveritem.cpp
	interfacemanager.cpp
    serverselector.cpp
	settingsbase.cpp
    bitarrayprogressbar.cpp
)
set(TrickleUi
    serversettings.ui
	generalsettings.ui
	torrentinfo.ui
    fileinfo.ui
	trackerinfo.ui
    serverinfo.ui
	servereditdialog.ui
)
set(TrickleKcfg settings.kcfgc)

kde4_add_ui_files(TrickleSources ${TrickleUi})
kde4_add_kcfg_files(TrickleSources ${TrickleKcfg})

kde4_add_executable(trickle ${TrickleSources})

target_link_libraries(trickle ${KDE4_KDEUI_LIBS} libtrickle)

install(FILES trickleui.rc DESTINATION ${DATA_INSTALL_DIR}/trickle)
install(FILES trickle.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(TARGETS trickle  ${INSTALL_TARGETS_DEFAULT_ARGS})

