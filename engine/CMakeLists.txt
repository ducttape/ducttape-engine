aux_source_directory(src engine_src)
aux_source_directory(src/component engine_src)
aux_source_directory(src/event engine_src)
aux_source_directory(src/game engine_src)
aux_source_directory(src/graphics engine_src)
aux_source_directory(src/input engine_src)
aux_source_directory(src/network engine_src)
aux_source_directory(src/resources engine_src)
aux_source_directory(src/scene engine_src)
aux_source_directory(src/states engine_src)
aux_source_directory(src/utils engine_src)
aux_source_directory(src/physics engine_src)

include_directories(src/)

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
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
