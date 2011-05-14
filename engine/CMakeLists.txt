aux_source_directory(src/ engine_src)

include_directories(src/)

add_library(engine STATIC
    ${engine_src}
)

target_link_libraries(engine
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OIS_IBRARIES}
)

