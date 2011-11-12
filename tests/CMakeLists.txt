# This file generates a test framework using all scripts in src/

file(GLOB tests_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src src/*.hpp)

file(GLOB_RECURSE tests_SOURCES src/*.cpp)
file(GLOB_RECURSE tests_HEADERS src/*.hpp)

include_directories(src/)

qt4_wrap_cpp(tests_HEADERS_MOC ${tests_HEADERS})

add_executable(test_framework
    ${tests_SOURCES}
    ${tests_HEADERS_MOC})

target_link_libraries(test_framework
    ducttape)

# Copy additional scripts needed to run tests.
file(GLOB SCRIPTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} scripts/*)
foreach(test_script ${SCRIPTS})
    string(REPLACE "scripts/" "" script ${test_script})
    configure_file(${test_script} ${PROJECT_SOURCE_DIR}/bin/${script} COPYONLY)
endforeach()

# We need to add test manually here instead of doing it in the loop because
# some tests need arguments while others do not.+

# core
add_test(NAME Random COMMAND test_framework Random)
add_test(NAME Logger COMMAND test_framework Logger)

# events
add_test(NAME Events COMMAND test_framework Events)
add_test(NAME EventBindings COMMAND test_framework EventBindings)
add_test(NAME Signals COMMAND test_framework Signals)
add_test(NAME Timer COMMAND test_framework Timer)

# logic
add_test(NAME Connections COMMAND test_framework Connections)
add_test(NAME Names COMMAND test_framework Names)
add_test(NAME QObject COMMAND test_framework QObject)
add_test(NAME Scripting COMMAND test_framework Scripting)
add_test(NAME ScriptComponent COMMAND test_framework ScriptComponent)
# disabled for Windows compatibility
# add_test(NAME Network COMMAND ${PROJECT_SOURCE_DIR}/bin/Network.sh)
add_test(NAME SerializationBinary COMMAND test_framework SerializationBinary)
add_test(NAME SerializationYaml COMMAND test_framework SerializationYaml)

# graphics
add_test(NAME Display COMMAND test_framework Display)
add_test(NAME ResourceManager COMMAND test_framework ResourceManager)
add_test(NAME Cameras COMMAND test_framework Cameras)
add_test(NAME States COMMAND test_framework States)
add_test(NAME Particles COMMAND test_framework Particles)
add_test(NAME FollowPath COMMAND test_framework FollowPath)
add_test(NAME Input COMMAND test_framework Input)
add_test(NAME Text COMMAND test_framework Text)
add_test(NAME Gui COMMAND test_framework Gui)
add_test(NAME MouseCursor COMMAND test_framework MouseCursor)
add_test(NAME Primitives COMMAND test_framework Primitives)
add_test(NAME Shadows COMMAND test_framework Shadows)
# add_test(NAME Terrain COMMAND test_framework Terrain)
add_test(NAME Billboard COMMAND test_framework Billboard)

# physics
add_test(NAME PhysicsSimple COMMAND test_framework PhysicsSimple)
# add_test(NAME PhysicsStress COMMAND test_framework PhysicsStress)

# audio
add_test(NAME MusicFade COMMAND test_framework MusicFade)
add_test(NAME Music COMMAND test_framework Music)
add_test(NAME Sound COMMAND test_framework Sound)

set(memorycheck_command:FILEPATH=/usr/bin/valgrind)
