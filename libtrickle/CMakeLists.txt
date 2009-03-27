SET(LibTrickleSources interface.cpp interfaceconfigwidget.cpp server.cpp xmlrpc.cpp bytesize.cpp bcodec.cpp csvcodec.cpp torrent.cpp)

KDE4_ADD_LIBRARY(libtrickle SHARED ${LibTrickleSources})

TARGET_LINK_LIBRARIES(libtrickle ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

INSTALL(FILES trickleinterface.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})
INSTALL(TARGETS libtrickle ${INSTALL_TARGETS_DEFAULT_ARGS})
SET_TARGET_PROPERTIES(libtrickle PROPERTIES OUTPUT_NAME trickle)