project(ducttape)

cmake_minimum_required(VERSION 2.8.3)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(BUILD_STATIC FALSE CACHE BOOL "TRUE to build static libraries, FALSE to build shared libraries (preferred)")

set(BUILD_EDITOR FALSE CACHE BOOL "TRUE to build the editor, FALSE to ignore it")

set(BUILD_SAMPLES TRUE CACHE BOOL "TRUE to build the samples, FALSE to ignore them")

set(BUILD_TESTS TRUE CACHE BOOL "TRUE to build the tests, FALSE to ignore them")

set(BUILD_DOC TRUE CACHE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

if(BUILD_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

find_package(SFML 2 COMPONENTS audio graphics network system window REQUIRED)
find_package(Bullet REQUIRED)
find_package(Boost COMPONENTS filesystem program_options system date_time REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(OIS REQUIRED)
find_package(OGRE REQUIRED)
find_package(MyGUI REQUIRED)
find_package(MyGUI_PLATFORM REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -std=c++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

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

if(BUILD_EDITOR)
    add_subdirectory("${CMAKE_SOURCE_DIR}/editor/")
endif()

if(BUILD_SAMPLES)
    add_subdirectory("${CMAKE_SOURCE_DIR}/samples/")
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory("${CMAKE_SOURCE_DIR}/tests/")
endif()

if(BUILD_DOC)
    add_subdirectory("${CMAKE_SOURCE_DIR}/docs/")
endif()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/engine/src/"
        DESTINATION include/ducttape/
        FILES_MATCHING PATTERN "*.hpp")
