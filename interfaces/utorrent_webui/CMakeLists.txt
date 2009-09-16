set(uTorrentSources utorrentinterface.cpp utorrentconfig.cpp)
set(uTorrentUi utorrentconfig.ui)

kde4_add_ui_files(uTorrentSources ${uTorrentUi})

kde4_add_plugin(trickle_utorrent ${uTorrentSources}) 

target_link_libraries(trickle_utorrent ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} libtrickle)

install(FILES trickle_utorrent.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(TARGETS trickle_utorrent DESTINATION ${PLUGIN_INSTALL_DIR})

