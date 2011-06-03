find_package(Doxygen REQUIRED)

configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_BINARY_DIR}/docs/Doxyfile)

configure_file(${CMAKE_SOURCE_DIR}/docs/logo.png ${CMAKE_BINARY_DIR}/docs/logo.png COPYONLY)

add_custom_target(doc 
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/docs/Doxyfile
    COMMENT "Building documentation" VERBATIM)

install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/
        DESTINATION share/ducttape)
