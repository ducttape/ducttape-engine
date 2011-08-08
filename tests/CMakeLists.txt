# This file generates a test framework using all scripts in src/

file(GLOB tests_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src src/*.hpp)

# Clean up last generated file
file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp)

file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
    "#include <cstring>\n")

foreach(test_hpp ${tests_HEADERS})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
         "#include \"${test_hpp}\"\n")
endforeach()

file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
    "int main(int argc, char** argv) {\n")

foreach(test_hpp ${tests_HEADERS})
    string(REPLACE ".hpp" "" test ${test_hpp})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
        "if(!strcmp(argv[1], \"${test}\"))\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
        "    return ${test}::Run(argc, argv);\n")
endforeach()

file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
    "}\n")

include_directories(src/)

file(GLOB_RECURSE tests_HEADERS src/*.hpp)
file(GLOB_RECURSE tests_SOURCES src/*.cpp)
qt4_wrap_cpp(tests_HEADERS_MOC ${tests_HEADERS})

add_executable(test_framework
    ${CMAKE_CURRENT_BINARY_DIR}/test_framework.cpp
    ${tests_SOURCES}
    ${tests_HEADERS_MOC})

target_link_libraries(test_framework
    ducttape)

# Copy additional scripts needed to run tests.
file(GLOB SCRIPTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} scripts/*)
foreach(test_script ${SCRIPTS})
    string(REPLACE "scripts/" "" script ${test_script})
    configure_file(${test_script} ${CMAKE_SOURCE_DIR}/bin/${script} COPYONLY)
endforeach()

# We need to add test manually here instead of doing it in the loop because
# some tests need arguments while others do not.
add_test(NAME test_random COMMAND test_framework test_random)
add_test(NAME test_logger COMMAND test_framework test_logger)
add_test(NAME test_events COMMAND test_framework test_events)
add_test(NAME test_eventbindings COMMAND test_framework test_eventbindings)
add_test(NAME test_signals COMMAND test_framework test_signals)
add_test(NAME test_timer COMMAND test_framework test_timer)
add_test(NAME test_connections COMMAND test_framework test_connections)
add_test(NAME test_names COMMAND test_framework test_names)
add_test(NAME test_qobject COMMAND test_framework test_qobject)
add_test(NAME test_script COMMAND test_framework test_script)
add_test(NAME test_scriptcomponent COMMAND test_framework test_scriptcomponent)
#add_test(NAME test_network ${CMAKE_SOURCE_DIR}/bin/test_network.sh)
add_test(NAME test_display COMMAND test_framework test_manager)
add_test(NAME test_resourcemanager COMMAND test_framework test_resourcemanager)
add_test(NAME test_multiple_camera COMMAND test_framework test_multiple_camera)
add_test(NAME test_particles COMMAND test_framework test_particles)
add_test(NAME test_followpath COMMAND test_framework test_followpath)
add_test(NAME test_input COMMAND test_framework text_input)
add_test(NAME test_text COMMAND test_framework test_text)
add_test(NAME test_gui COMMAND test_framework test_gui)
add_test(NAME test_mousecursor COMMAND test_framework test_mousecursor)
add_test(NAME test_primitive_mesh COMMAND test_framework test_primitive_mesh)
add_test(NAME test_shadow COMMAND test_framework test_shadow)
add_test(NAME test_physics COMMAND test_framework test_physics)
add_test(NAME test_sound COMMAND test_framework test_sound)
add_test(NAME test_music COMMAND test_framework test_music)
add_test(NAME test_music_fade COMMAND test_framework test_music_fade)

set(memorycheck_command:FILEPATH=/usr/bin/valgrind)
