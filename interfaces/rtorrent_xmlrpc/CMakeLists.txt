SET(rTorrentSources rtorrentinterface.cpp rtorrentconfig.cpp xmlrpc.cpp)
SET(rTorrentUi rtorrentconfig.ui)

KDE4_ADD_UI_FILES(rTorrentSources ${rTorrentUi})

KDE4_ADD_PLUGIN(trickle_rtorrent ${rTorrentSources}) 

TARGET_LINK_LIBRARIES(trickle_rtorrent ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} libtrickle)

INSTALL(FILES trickle_rtorrent.desktop DESTINATION ${SERVICES_INSTALL_DIR})
INSTALL(TARGETS trickle_rtorrent DESTINATION ${PLUGIN_INSTALL_DIR})
