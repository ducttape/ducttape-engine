project(ducttape-engine)

cmake_minimum_required(VERSION 2.8.3)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(BUILD_STATIC FALSE CACHE BOOL "TRUE to build static libraries (mind LGPL3 implications), FALSE to build shared libraries (preferred)")
set(BUILD_SAMPLES TRUE CACHE BOOL "TRUE to build the samples, FALSE to ignore them")
set(BUILD_TESTS TRUE CACHE BOOL "TRUE to build the tests, FALSE to ignore them")
set(BUILD_DOC TRUE CACHE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(BULLET_ROOT "" CACHE PATH "Path to Bullet dir")
set(BOOST_ROOT "" CACHE PATH "Path to Boost dir")
set(OGRE_HOME "" CACHE PATH "Path to OGRE dir")
set(SFMLDIR "" CACHE PATH "Path to SFML dir")
set(ENV{QTDIR} "" CACHE PATH "Path to Qt4")
set(MYGUI_ROOT "" CACHE PATH "Path to MYGUI dir")
set(OgreProcedural_HOME "" CACHE PATH "Path to Ogre Procedural dir")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

if(EXISTS "${CMAKE_SOURCE_DIR}/dependencies")
    set(BULLET_ROOT "${CMAKE_SOURCE_DIR}/dependencies/bullet")
    set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/dependencies/boost")
    set(OGRE_HOME "${CMAKE_SOURCE_DIR}/dependencies/ogre")
    set(SFMLDIR "${CMAKE_SOURCE_DIR}/dependencies/sfml")
    set(ENV{QTDIR} "${CMAKE_SOURCE_DIR}/dependencies/qt4")
    file(WRITE "${CMAKE_SOURCE_DIR}/dependencies/qt4/bin/qt.conf" "[Paths]\nPrefix = ${CMAKE_SOURCE_DIR}/dependencies/qt4")
    set(MYGUI_ROOT "${CMAKE_SOURCE_DIR}/dependencies/mygui")
    set(OgreProcedural_HOME "${CMAKE_SOURCE_DIR}/dependencies/ogre-procedural")
    file(GLOB_RECURSE DLLS_LIST ${CMAKE_SOURCE_DIR}/dependencies/*.dll)
    file(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")
    file(COPY ${DLLS_LIST} DESTINATION "${EXECUTABLE_OUTPUT_PATH}/Debug")
    file(COPY ${DLLS_LIST} DESTINATION "${EXECUTABLE_OUTPUT_PATH}/Release")
endif()

find_package(SFML 2 COMPONENTS audio graphics network system window REQUIRED)
find_package(Bullet REQUIRED)
find_package(Boost COMPONENTS date_time program_options thread REQUIRED)
find_package(OIS REQUIRED)
find_package(OGRE REQUIRED)
find_package(MyGUI REQUIRED)
find_package(MyGUI_PLATFORM REQUIRED)

# Qt4 stuff
set(QT_USE_QTSCRIPT TRUE)
set(QT_DONT_USE_QTGUI TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITONS})

if(MSVC) # Sort the odd stuff out
    set(CMAKE_CXX_FLAGS "/DCOMPILER_MSVC /DOgreProcedural_STATIC /WX /MP /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /GL /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
else() # GCC/Mingw
    set(CMAKE_CXX_FLAGS "-DCOMPILER_GCC -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-parameter -pedantic -std=c++0x -fPIC")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/engine/src/
    ${Boost_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
    ${OGRE_INCLUDE_DIRS}
    ${MYGUI_INCLUDE_DIR}
    ${MYGUI_PLATFORM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
    ${OIS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/externals/btogre/include
    ${CMAKE_SOURCE_DIR}/externals/ogre-procedural/library/include
)

add_subdirectory(${CMAKE_SOURCE_DIR}/externals)

add_subdirectory("${CMAKE_SOURCE_DIR}/engine/")

if(BUILD_EDITOR)
    add_subdirectory("${CMAKE_SOURCE_DIR}/editor/")
endif()

if(BUILD_SAMPLES)
    add_subdirectory("${CMAKE_SOURCE_DIR}/samples/")
endif()

if(BUILD_TESTS)
    #include(CTest)
    enable_testing()
    add_subdirectory("${CMAKE_SOURCE_DIR}/tests/")
endif()

if(BUILD_DOC)
    add_subdirectory("${CMAKE_SOURCE_DIR}/docs/")
endif()

# Installation stuff.
install(DIRECTORY "${CMAKE_SOURCE_DIR}/engine/src/"
        DESTINATION include/ducttape
        FILES_MATCHING PATTERN "*.hpp")

install(FILES "${CMAKE_SOURCE_DIR}/cmake-extras/FindDucttape.cmake"
        DESTINATION ${CMAKE_ROOT}/Modules)

install(FILES "${CMAKE_SOURCE_DIR}/LICENSE"
        DESTINATION share/ducttape)
