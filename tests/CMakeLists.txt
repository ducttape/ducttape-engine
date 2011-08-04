# Make tests.
aux_source_directory(src TESTS)
foreach(test_cpp ${TESTS})
    # string format: "src//testname.cpp" > "testname"
    string(REGEX REPLACE "\\.cpp$" "" test ${test_cpp})
    string(REGEX REPLACE "src/" "" test ${test})

    add_executable(${test}
        "src/"${test_cpp})

    target_link_libraries(${test}
        ducttape)
endforeach(test_cpp)

# Copy additional scripts needed to run tests.
file(GLOB SCRIPTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} scripts/*)
foreach(test_script ${SCRIPTS})
    string(REGEX REPLACE "scripts/" "" script ${test_script})
    configure_file(${test_script} ${CMAKE_SOURCE_DIR}/bin/${script} COPYONLY)
endforeach()

# We need to add test manually here instead of doing it in the loop because
# some tests need arguments while others do not.
add_test(test_random ${CMAKE_SOURCE_DIR}/bin/test_random)
add_test(test_logger ${CMAKE_SOURCE_DIR}/bin/test_logger)
add_test(test_events ${CMAKE_SOURCE_DIR}/bin/test_events)
add_test(test_eventbindings ${CMAKE_SOURCE_DIR}/bin/test_eventbindings)
add_test(test_signals ${CMAKE_SOURCE_DIR}/bin/test_signals)
add_test(test_timer ${CMAKE_SOURCE_DIR}/bin/test_timer)
add_test(test_connections ${CMAKE_SOURCE_DIR}/bin/test_connections)
add_test(test_names ${CMAKE_SOURCE_DIR}/bin/test_names)
add_test(test_qobject ${CMAKE_SOURCE_DIR}/bin/test_qobject)
add_test(test_script ${CMAKE_SOURCE_DIR}/bin/test_script)
add_test(test_network ${CMAKE_SOURCE_DIR}/bin/test_network.sh)
add_test(test_display ${CMAKE_SOURCE_DIR}/bin/test_display)
add_test(test_resourcemanager ${CMAKE_SOURCE_DIR}/bin/test_resourcemanager)
add_test(test_multiple_camera ${CMAKE_SOURCE_DIR}/bin/test_multiple_camera)
add_test(test_particles ${CMAKE_SOURCE_DIR}/bin/test_particles)
add_test(test_followpath ${CMAKE_SOURCE_DIR}/bin/test_followpath)
add_test(test_input ${CMAKE_SOURCE_DIR}/bin/test_input)
add_test(test_text ${CMAKE_SOURCE_DIR}/bin/test_text)
add_test(test_gui ${CMAKE_SOURCE_DIR}/bin/test_gui)
add_test(test_mousecursor ${CMAKE_SOURCE_DIR}/bin/test_mousecursor)
add_test(test_primitive_mesh ${CMAKE_SOURCE_DIR}/bin/test_primitive_mesh)
add_test(test_shadow ${CMAKE_SOURCE_DIR}/bin/test_shadow)
add_test(test_physics ${CMAKE_SOURCE_DIR}/bin/test_physics)
add_test(test_sound ${CMAKE_SOURCE_DIR}/bin/test_sound)
add_test(test_music ${CMAKE_SOURCE_DIR}/bin/test_music)
add_test(test_music_fade ${CMAKE_SOURCE_DIR}/bin/test_music_fade)

set(memorycheck_command:FILEPATH=/usr/bin/valgrind)
