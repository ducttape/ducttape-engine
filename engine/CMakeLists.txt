aux_source_directory(src/Audio engine_src)
aux_source_directory(src/Core engine_src)
aux_source_directory(src/Event engine_src)
aux_source_directory(src/Graphics engine_src)
aux_source_directory(src/Input engine_src)
aux_source_directory(src/Logic engine_src)
aux_source_directory(src/Network engine_src)
aux_source_directory(src/Physics engine_src)
aux_source_directory(src/Scene engine_src)
aux_source_directory(src/Utils engine_src)

include_directories(src/)

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
    add_definitions(-DDUCTTAPE_STATIC)
else()
    set(BUILD_MODE SHARED)
endif()

add_library(ducttape ${BUILD_MODE}
    ${engine_src}
)

target_link_libraries(ducttape
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    BtOgre
    OgreProcedural
)

set_target_properties(ducttape
    PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

install(TARGETS ducttape
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
