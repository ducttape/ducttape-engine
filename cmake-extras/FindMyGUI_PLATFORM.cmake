###############################################################################
# MyGUI OgrePlatform
###############################################################################

findpkg_begin ( "MYGUI_PLATFORM" )

set ( MYGUI_PLATFORM_LIBRARY_NAMES MyGUI.OgrePlatform )
get_debug_names ( MYGUI_PLATFORM_LIBRARY_NAMES )

find_path ( MYGUI_PLATFORM_INCLUDE_DIR
  MyGUI_OgrePlatform.h PATHS
  ${MYGUI_ROOT}/Platforms/Ogre/OgrePlatform/include
  "/usr/include/MYGUI"
  "/usr/local/include/MYGUI"
  "/usr/include/OGRE"
  "/usr/local/include/OGRE"
)

find_library ( MYGUI_PLATFORM_LIBRARY_REL
  ${MYGUI_PLATFORM_LIBRARY_NAMES} PATHS
  ${MYGUI_ROOT}/lib/Release
  ${MYGUI_ROOT}/lib/RelWithDebInfo
  ${MYGUI_ROOT}/lib/MinSizeRel
  "/usr/lib"
  "/usr/local/lib"
)

find_library ( MYGUI_PLATFORM_LIBRARY_DBG
  ${MYGUI_PLATFORM_LIBRARY_NAMES_DBG} PATHS
  ${MYGUI_ROOT}/lib/Debug
  "/usr/lib"
  "/usr/local/lib"
)

make_library_set ( MYGUI_PLATFORM_LIBRARY )

set ( MYGUI_PLATFORM_FOUND TRUE )
if ( NOT MYGUI_PLATFORM_LIBRARY OR NOT MYGUI_PLATFORM_INCLUDE_DIR )
  message ( FATAL_ERROR "MyGUI OgrePlatform library or include directory could not be located" )
  set ( MYGUI_PLATFORM_FOUND FALSE )
endif ()

if ( NOT MYGUI_PLATFORM_FOUND  )
  set ( MYGUI_LIBRARIES "MYGUI_PLATFORM-NOTFOUND")
else ()
  set ( MYGUI_LIBRARIES ${MYGUI_LIBRARY} ${MYGUI_PLATFORM_LIBRARY} )
    
  set ( MYGUI_INCLUDE_DIRS
    "${MYGUI_INCLUDE_DIR}"
    "${MYGUI_PLATFORM_INCLUDE_DIR}" )
endif ()

findpkg_finish ( MYGUI_PLATFORM )
