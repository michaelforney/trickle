if(ENABLE_RTORRENT)
    add_subdirectory(rtorrent_xmlrpc)
endif(ENABLE_RTORRENT)
if(ENABLE_UTORRENT)
    add_subdirectory(utorrent_webui)
endif(ENABLE_UTORRENT)

