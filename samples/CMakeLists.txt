
file(GLOB_RECURSE samples_client_HEADERS chat/src/client/*.hpp)
file(GLOB_RECURSE samples_server_HEADERS chat/src/server/*.hpp)
file(GLOB_RECURSE simplepong_HEADERS simplepong/src/*.hpp)
file(GLOB_RECURSE advancedpong_HEADERS advancedpong/src/*.hpp)
file(GLOB_RECURSE jumptape_HEADERS jumptape/src/*.hpp)
file(GLOB_RECURSE sample_fps_HEADERS fps/src/*.hpp)

qt4_wrap_cpp(samples_client_HEADERS_MOC ${samples_client_HEADERS})
qt4_wrap_cpp(samples_server_HEADERS_MOC ${samples_server_HEADERS})
qt4_wrap_cpp(simplepong_HEADERS_MOC ${simplepong_HEADERS})
qt4_wrap_cpp(advancedpong_HEADERS_MOC ${advancedpong_HEADERS})
qt4_wrap_cpp(jumptape_HEADERS_MOC ${jumptape_HEADERS})
qt4_wrap_cpp(fps_HEADERS_MOC ${sample_fps_HEADERS})

set(NETWORKSAMPLES chat)

foreach(sample ${NETWORKSAMPLES})
    aux_source_directory(${sample}/src/client ${sample}_client_src)
    aux_source_directory(${sample}/src/common ${sample}_common_src)
    aux_source_directory(${sample}/src/server ${sample}_server_src)
    aux_source_directory(${sample}/src ${sample}_src)

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

set(SIMPLESAMPLES simplepong advancedpong script jumptape fps)

foreach(sample ${SIMPLESAMPLES})
    aux_source_directory(${sample}/src ${sample}_src)

    include_directories(
        ${sample}/src)

    add_executable(sample_${sample}
        ${${sample}_src}
        ${${sample}_HEADERS_MOC})

    target_link_libraries(sample_${sample}
        ducttape)
endforeach(sample)
