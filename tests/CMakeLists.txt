set(TESTS events connections logger network)

FOREACH(test ${TESTS})
    aux_source_directory(${test} ${test}_src)

    include_directories(
        ${test}
    )

    add_executable(test_${test}
        ${${test}_src}
    )

    target_link_libraries(test_${test}
        engine
    )

ENDFOREACH(test)
