
file(GLOB_RECURSE samples_client_HEADERS chat/src/client/*.hpp)
file(GLOB_RECURSE samples_server_HEADERS chat/src/server/*.hpp)

qt4_wrap_cpp(samples_client_HEADERS_MOC ${samples_client_HEADERS})
qt4_wrap_cpp(samples_server_HEADERS_MOC ${samples_server_HEADERS})

set(NETWORKSAMPLES chat)

foreach(sample ${NETWORKSAMPLES})
    aux_source_directory(${sample}/src/client ${sample}_client_src)
    aux_source_directory(${sample}/src/common ${sample}_common_src)
    aux_source_directory(${sample}/src/server ${sample}_server_src)

    include_directories(
        ${sample}/src/client
        ${sample}/src/common
        ${sample}/src/server)

    add_executable(sample_${sample}_client
        ${${sample}_client_src}
        ${${sample}_common_src}
        ${samples_client_HEADERS_MOC})

    add_executable(sample_${sample}_server
        ${${sample}_server_src}
        ${${sample}_common_src}
        ${samples_server_HEADERS_MOC})

    target_link_libraries(sample_${sample}_client
        ducttape)

    target_link_libraries(sample_${sample}_server
        ducttape)
endforeach(sample)

set(SIMPLESAMPLES simplepong script)

foreach(sample ${SIMPLESAMPLES})
    aux_source_directory(${sample}/src ${sample}_src)

    include_directories(
        ${sample}/src)

    add_executable(sample_${sample}
        ${${sample}_src})

    target_link_libraries(sample_${sample}
        ducttape)
endforeach(sample)
