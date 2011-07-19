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
add_test(test_timer ${CMAKE_SOURCE_DIR}/bin/test_timer)
add_test(test_connections ${CMAKE_SOURCE_DIR}/bin/test_connections)
add_test(test_network ${CMAKE_SOURCE_DIR}/bin/test_network.sh)
add_test(test_display ${CMAKE_SOURCE_DIR}/bin/test_display)
add_test(test_music ${CMAKE_SOURCE_DIR}/bin/test_music)
add_test(test_sound ${CMAKE_SOURCE_DIR}/bin/test_sound)
add_test(test_multiple_camera ${CMAKE_SOURCE_DIR}/bin/test_multiple_camera)
add_test(test_particles ${CMAKE_SOURCE_DIR}/bin/test_particles)
add_test(test_names ${CMAKE_SOURCE_DIR}/bin/test_names)

set(memorycheck_command:FILEPATH=/usr/bin/valgrind)
