aux_source_directory(src/ TESTS)

foreach(test_cpp ${TESTS})
    # string format: "src//testname.cpp" > "testname"
    string(REGEX REPLACE "\\.cpp$" "" test ${test_cpp})
    string(REGEX REPLACE "src//" "" test ${test})

    add_executable(${test}
        "src/"${test_cpp}
    )

    target_link_libraries(${test}
        engine
    )
endforeach(test_cpp)
