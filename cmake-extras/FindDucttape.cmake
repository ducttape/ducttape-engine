# Locate the Ducttape Game Engine
#
# This module defines
#  DUCTTAPE_FOUND, if false, do not try to link to Ducttape
#  DUCTTAPE_LIBRARY, where to find the Ducttape library
#  DUCTTAPE_INCLUDE_DIR, where to find ducttape/Root.hpp
#
# By default, the dynamic libraries of Ducttape will be found. To find the static ones instead,
# you must set the DUCTTAPE_STATIC_LIBRARY variable to TRUE before calling find_package(Ducttape ...).
#
# If Ducttape is not installed in a standard path, you can use the DUCTTAPE_DIR CMake variable
# to tell CMake where Ducttape is.

# attempt to find static library first if this is set
if(DUCTTAPE_STATIC_LIBRARY)
    set(DUCTTAPE_STATIC libducttape.a)
endif()

# find the Ducttape include directory
find_path(DUCTTAPE_INCLUDE_DIR Root.hpp
          PATH_SUFFIXES include
          PATHS
          ~/Library/Frameworks/ducttape/
          /Library/Frameworks/ducttape/
          /usr/local/ducttape/
          /usr/ducttape/
          /sw/ducttape/         # Fink
          /opt/local/ducttape/  # DarwinPorts
          /opt/csw/ducttape/    # Blastwave
          /opt/ducttape/
          ${DUCTTAPE_DIR}/src/)

# find the Ducttape library
find_library(DUCTTAPE_LIBRARY
             NAMES ${DUCTTAPE_STATIC} ducttape
             PATH_SUFFIXES lib64 lib
             PATHS ~/Library/Frameworks
                    /Library/Frameworks
                    /usr/local
                    /usr
                    /sw
                    /opt/local
                    /opt/csw
                    /opt
                    ${DUCTTAPE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set DUCTTAPE_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DUCTTAPE DEFAULT_MSG DUCTTAPE_INCLUDE_DIR DUCTTAPE_LIBRARY)
mark_as_advanced(DUCTTAPE_INCLUDE_DIR DUCTTAPE_LIBRARY)
