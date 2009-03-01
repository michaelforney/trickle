SET(LibTrickleSources interface.cpp interfaceconfigwidget.cpp server.cpp xmlrpc.cpp bytesize.cpp bcodec.cpp csvcodec.cpp torrent.cpp)

KDE4_ADD_LIBRARY(trickle SHARED ${LibTrickleSources})

TARGET_LINK_LIBRARIES(trickle ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

INSTALL(FILES trickleinterface.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})
INSTALL(TARGETS trickle ${INSTALL_TARGETS_DEFAULT_ARGS})
