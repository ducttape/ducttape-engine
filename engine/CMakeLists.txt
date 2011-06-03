aux_source_directory(src/ engine_src)
aux_source_directory(src/component/ engine_src)
aux_source_directory(src/event/ engine_src)
aux_source_directory(src/network/ engine_src)
aux_source_directory(src/scene/ engine_src)
aux_source_directory(src/states/ engine_src)
aux_source_directory(src/utils/ engine_src)

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

