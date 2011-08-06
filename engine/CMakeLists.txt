# Gather sources
file(GLOB_RECURSE engine_SOURCES src/*.cpp)

# Gather headers and run them through moc
file(GLOB_RECURSE engine_HEADERS src/*.hpp)
qt4_wrap_cpp(engine_HEADERS_MOC ${engine_HEADERS})

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
    add_definitions(-DDUCTTAPE_STATIC)
else()
    set(BUILD_MODE SHARED)
endif()

add_definitions(-DDUCTTAPE_VERSION="${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

add_library(ducttape ${BUILD_MODE}
    ${engine_SOURCES}
    ${engine_HEADERS_MOC}
)

target_link_libraries(ducttape
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    ${QT_LIBRARIES}
    BtOgre
    OgreProcedural
)

set_target_properties(ducttape
    PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

install(TARGETS ducttape
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
