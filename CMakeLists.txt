cmake_minimum_required( VERSION 2.8 )

project(FXMediaInfo)

set( APP fxmediainfo)

file(GLOB SOURCES "*.cxx" "*.hpp")

set ( CMAKE_CXX_COMPILER "g++" )

set( CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -DUNICODE -std=c++11 -Ofast -Wl,-s")

set( CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -DUNICODE -Wall -std=c++11 -O2 -ggdb -pg -lc")


include (FindPkgConfig)
pkg_search_module(FOX REQUIRED fox17)
pkg_search_module(LIBMEDIAINFO REQUIRED libmediainfo)


include_directories(
    ${FOX_INCLUDE_DIRS} 
    )

link_directories(
    ${FOX_LIBRARY_DIRS} 
    )


add_executable( 
    ${APP} 
    ${SOURCES} 
    ${FOX_INCLUDE_DIRS} 
    ${LIBMEDIAINFO_INCLUDE_DIRS}
    )

target_link_libraries( 
    ${APP} 
     ${FOX_LIBRARIES} 
     ${FOX_STATIC_LIBRARIES} 
     ${LIBMEDIAINFO_LIBRARIES} 
    )
