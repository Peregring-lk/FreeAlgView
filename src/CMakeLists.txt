cmake_minimum_required(VERSION 2.8.9)

project(FreeAlgView CXX)

set(CMAKE_CXX_COMPILER /usr/lib/gcc-snapshot/bin/g++)
#set(CMAKE_CXX_COMPILER /usr/bin/g++)

##
## Declaring directories.
##
set(src_dir           ${CMAKE_CURRENT_SOURCE_DIR})

set(fau_dir           ${src_dir}/freealgview)
set(faupp_dir         ${fau_dir}/faupp)

set(parser_dir        ${faupp_dir})
set(lexer_dir         ${parser_dir}/lexer)

set(debug_dir         ${fau_dir}/debugger)
set(sdebug_dir        ${fau_dir}/simple_debugger)

set(gdebug_dir        ${fau_dir}/graphic_debugger)
set(gdebug_h_dir      ${gdebug_dir}/helpers)

##### Phantom setting.
set(phantom_dir ${src_dir}/.phantom-dir)
include(${phantom_dir}/phantom.cmake)
#####

find_package(Qt4 4.8.3 REQUIRED)
#find_package(Qt5Core REQUIRED)
find_package(Bisonc++ 4.01.00 REQUIRED)
find_package(Flexc++ 1.01.00 REQUIRED)

# Find GraphViz
find_library(GVC gvc)
find_library(GRAPH graph)
find_library(CDT cdt)

set(GRAPHVIZ_LIBRARIES ${GVC} ${GRAPH} ${CDT})

# Find FTGL
find_library(FTGL ftgl)
MESSAGE("Lib: ${FTGL}")
MESSAGE("Found Lib: ${FTGL_LIB}")

include_directories(/usr/include/freetype2/)

## Setting compilation options
set(Flexc++_args "--no-lines")
set(Bisonc++_args "--no-lines")

# C++0x ... and others c++ flags
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-std=c++11 -ggdb")
#    set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -ggdb -Wno-write-strings")
endif()

##### Phantoms call.
call_phantoms()
#####

##
## Declaring source files.
##

set(gdebug_sources    ${gdebug_dir}/graphic_debugger.cpp
                      ${gdebug_dir}/layouter.cpp
                      ${gdebug_dir}/mouse.cpp
                      ${gdebug_dir}/scene.cpp
                      ${gdebug_dir}/viewer.cpp
                      ${gdebug_h_dir}/smoother.cpp
                      ${gdebug_h_dir}/vecxf.cpp
)

set(debug_sources     ${debug_dir}/debugger.cpp
                      ${sdebug_dir}/simple_debugger.cpp
                      ${gdebug_sources}
)

set(faupp_sources     ${lexer_dir}/lexer.cpp
                      ${parser_dir}/faupp.cpp
)

set(fau_sources       ${fau_dir}/freealgview.cpp
)

set(all_sources       ${debug_sources} ${faupp_sources} ${fau_sources}
                      ${src_dir}/main.cpp
)

# Defining executable.
add_executable(FreeAlgView ${all_sources} ${fau_s_phantoms})

target_link_libraries(FreeAlgView ${QT_LIBRARIES} ${GRAPHVIZ_LIBRARIES}
  ${FTGL} -lGL -lGLU)
