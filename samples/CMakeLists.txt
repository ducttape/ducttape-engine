set(SAMPLES skeleton)

FOREACH(sample ${SAMPLES})
    aux_source_directory(${sample}/src/client ${sample}_client_src)
    aux_source_directory(${sample}/src/common ${sample}_common_src)
    aux_source_directory(${sample}/src/server ${sample}_server_src)

    include_directories(
        ${sample}/src/client
        ${sample}/src/common
        ${sample}/src/server
        )

    add_executable(sample_${sample}_client
        ${${sample}_client_src}
        ${${sample}_common_src}
    )

    add_executable(sample_${sample}_server
        ${${sample}_server_src}
        ${${sample}_common_src}
    )

    target_link_libraries(sample_${sample}_client
        engine
    )

    target_link_libraries(sample_${sample}_server
        engine
    )
ENDFOREACH(sample)

