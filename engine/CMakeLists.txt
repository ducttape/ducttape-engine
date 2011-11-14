# Gather sources
file(GLOB_RECURSE engine_SOURCES src/*.cpp)

# Gather headers and run them through moc
file(GLOB_RECURSE engine_HEADERS src/*.hpp)
qt4_wrap_cpp(engine_HEADERS_MOC ${engine_HEADERS})

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
else()
    set(BUILD_MODE SHARED)
endif()

add_definitions(-DDUCTTAPE_VERSION_MAJOR=${VERSION_MAJOR})
add_definitions(-DDUCTTAPE_VERSION_MINOR=${VERSION_MINOR})
add_definitions(-DDUCTTAPE_VERSION_PATCH=${VERSION_PATCH})

add_library(ducttape ${BUILD_MODE}
    ${engine_SOURCES}
    ${engine_HEADERS_MOC}
)

target_link_libraries(ducttape
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${OGRE_Paging_LIBRARIES}
    ${OGRE_Terrain_LIBRARIES}
    ${OIS_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    ${YAMLCPP_LIBRARY}
    ${QT_LIBRARIES}
    BtOgre
    OgreProcedural
    ogre-paged
)

set_target_properties(ducttape
    PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

install(TARGETS ducttape
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
