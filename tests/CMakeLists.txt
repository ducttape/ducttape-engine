aux_source_directory(src/ TESTS)

include_directories(
    ${test}
)

FOREACH(test_cpp ${TESTS})
    # string format: "src//testname.cpp" > "testname"
    STRING(REGEX REPLACE "\\.cpp$" "" test ${test_cpp})
    STRING(REGEX REPLACE "src//" "" test ${test})

    add_executable(test_${test}
        "src/"${test_cpp}
    )

    target_link_libraries(test_${test}
        engine
    )
ENDFOREACH(test_cpp)
