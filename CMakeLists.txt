project(trickle)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

set(TrickleIncludes ${CMAKE_SOURCE_DIR}/libtrickle)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${TrickleIncludes})

add_subdirectory(libtrickle)
add_subdirectory(trickle)
add_subdirectory(interfaces)
add_subdirectory(icons)

