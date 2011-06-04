aux_source_directory(src/ engine_src)
aux_source_directory(src/component/ engine_src)
aux_source_directory(src/event/ engine_src)
aux_source_directory(src/game/ engine_src)
aux_source_directory(src/network/ engine_src)
aux_source_directory(src/resources/ engine_src)
aux_source_directory(src/scene/ engine_src)
aux_source_directory(src/states/ engine_src)
aux_source_directory(src/utils/ engine_src)
aux_source_directory(src/graphics/ engine_src)

include_directories(src/)

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
else()
    set(BUILD_MODE SHARED)
endif()

add_library(engine ${BUILD_MODE}
    ${engine_src}
)

target_link_libraries(engine
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${OIS_IBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
)

