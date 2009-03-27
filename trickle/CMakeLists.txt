SET(TrickleSources main.cpp
	filemodel.cpp
	directoryitem.cpp
	filemodelitem.cpp
	trackerinfo.cpp
	trackermodel.cpp
	trackeritem.cpp
	trickle.cpp
	log.cpp
	updatetimer.cpp
	torrentwidget.cpp
	torrentmodel.cpp
	torrentview.cpp
	torrentinfo.cpp
	fileinfo.cpp
	fileview.cpp
	servermodel.cpp
	torrentsortmodel.cpp
	filedelegate.cpp
	torrentdelegate.cpp
	#selectedtorrent.cpp
	settingsdelegate.cpp
	generalinfo.cpp
	serverinfo.cpp
	serversettings.cpp
	generalsettings.cpp
	#servermanager.cpp
	configdialog.cpp
	servereditdialog.cpp
	serveritem.cpp
	interfacemanager.cpp
	settingsbase.cpp)
SET(TrickleUi serversettings.ui
	generalsettings.ui
	torrentinfo.ui
	trackerinfo.ui
	servereditdialog.ui)
SET(TrickleKcfg settings.kcfgc)

KDE4_ADD_UI_FILES(TrickleSources ${TrickleUi})
KDE4_ADD_KCFG_FILES(TrickleSources ${TrickleKcfg})

KDE4_ADD_EXECUTABLE(trickle ${TrickleSources})

TARGET_LINK_LIBRARIES(trickle ${KDE4_KDEUI_LIBS} libtrickle)

INSTALL(FILES trickleui.rc DESTINATION ${DATA_INSTALL_DIR}/trickle)
INSTALL(FILES trickle.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
INSTALL(TARGETS trickle  ${INSTALL_TARGETS_DEFAULT_ARGS})
