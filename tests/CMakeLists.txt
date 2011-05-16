set(TESTS events)

FOREACH(test ${TESTS})
    aux_source_directory(${test}/src ${test}_src)

    include_directories(
        ${test}/src
        )

    add_executable(test_${test}
        ${${test}_src}
    )

    target_link_libraries(test_${test}
        engine
    )

ENDFOREACH(test)

