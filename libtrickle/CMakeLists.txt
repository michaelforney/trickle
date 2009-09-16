set(LibTrickleSources interface.cpp interfaceconfig.cpp interfaceconfigwidget.cpp server.cpp bytesize.cpp bcodec.cpp file.cpp csvcodec.cpp torrent.cpp logdata.cpp logentry.cpp peer.cpp tracker.cpp)

KDE4_ADD_LIBRARY(libtrickle SHARED ${LibTrickleSources})

target_link_libraries(libtrickle ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(FILES trickleinterface.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(TARGETS libtrickle ${INSTALL_TARGETS_DEFAULT_ARGS})
set_target_properties(libtrickle PROPERTIES OUTPUT_NAME trickle)

