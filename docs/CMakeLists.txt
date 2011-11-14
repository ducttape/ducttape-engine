find_package(Doxygen)

configure_file(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/docs/Doxyfile)

configure_file(${PROJECT_SOURCE_DIR}/docs/logo.png ${PROJECT_BINARY_DIR}/docs/logo.png COPYONLY)

add_custom_target(doc 
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/docs/Doxyfile
    COMMENT "Building documentation" VERBATIM)

install(DIRECTORY ${PROJECT_BINARY_DIR}/docs/
        DESTINATION share/doc/ducttape
        PATTERN CMakeFiles EXCLUDE
        PATTERN cmake_install.cmake EXCLUDE
        PATTERN Doxyfile EXCLUDE
        PATTERN Makefile EXCLUDE)
