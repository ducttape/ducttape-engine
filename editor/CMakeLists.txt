aux_source_directory(src/common editor_common_src)
aux_source_directory(src/server editor_server_src)
aux_source_directory(src/client editor_client_src)

include_directories(
    ${editor_common_src}
    ${editor_client_src}
    ${editor_server_src}
    )

add_executable(editor_server
    ${editor_common_src}
    ${editor_server_src}
)

target_link_libraries(editor_server
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OIS_IBRARIES}
)

add_executable(editor_client
    ${editor_common_src}
    ${editor_client_src}
)

target_link_libraries(editor_client
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OIS_IBRARIES}
)
