project(ducttape-engine)

cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(SFML 2 COMPONENTS audio graphics network system window REQUIRED)
find_package(Bullet REQUIRED)
find_package(Boost COMPONENTS filesystem program_options system date_time REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(OIS REQUIRED)
find_package(OGRE REQUIRED)
find_package(MyGUI REQUIRED)
find_package(MyGUI_PLATFORM REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -fPIC -std=c++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(
    ${CMAKE_SOURCE_DIR}/engine/src/
    ${Boost_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}
    ${MYGUI_INCLUDE_DIR}
    ${MYGUI_PLATFORM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
    ${OIS_INCLUDE_DIRS}
)

add_subdirectory("${CMAKE_SOURCE_DIR}/engine/")
add_subdirectory("${CMAKE_SOURCE_DIR}/editor/")
add_subdirectory("${CMAKE_SOURCE_DIR}/samples/")
add_subdirectory("${CMAKE_SOURCE_DIR}/tests/")
