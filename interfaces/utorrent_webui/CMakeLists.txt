SET(uTorrentSources utorrentinterface.cpp utorrentconfig.cpp)
SET(uTorrentUi utorrentconfig.ui)

KDE4_ADD_UI_FILES(uTorrentSources ${uTorrentUi})

KDE4_ADD_PLUGIN(trickle_utorrent ${uTorrentSources}) 

TARGET_LINK_LIBRARIES(trickle_utorrent ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} trickle)

INSTALL(FILES trickle_utorrent.desktop DESTINATION ${SERVICES_INSTALL_DIR})
INSTALL(TARGETS trickle_utorrent DESTINATION ${PLUGIN_INSTALL_DIR})
