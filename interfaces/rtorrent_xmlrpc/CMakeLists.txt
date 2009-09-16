set(rTorrentSources rtorrentinterface.cpp rtorrentconfig.cpp rtorrentconfigwidget.cpp)
set(rTorrentUi rtorrentconfigwidget.ui)

kde4_add_ui_files(rTorrentSources ${rTorrentUi})

kde4_add_plugin(trickle_rtorrent ${rTorrentSources}) 

target_link_libraries(trickle_rtorrent ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} libtrickle)

install(FILES trickle_rtorrent.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(TARGETS trickle_rtorrent DESTINATION ${PLUGIN_INSTALL_DIR})

