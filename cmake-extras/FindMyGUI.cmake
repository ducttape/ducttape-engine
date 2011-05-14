###############################################################################
# MyGUI
###############################################################################

include( "${CMAKE_MODULE_PATH}/FindPkgMacros.cmake" )

findpkg_begin ( "MYGUI" )

set ( MYGUI_LIBRARY_NAMES MyGUIEngine )
get_debug_names ( MYGUI_LIBRARY_NAMES )

find_path ( MYGUI_INCLUDE_DIR
  MyGUI.h PATHS
  "/usr/include/MYGUI"
  "/usr/local/include/MYGUI"
  )

find_library ( MYGUI_LIBRARY_REL
  ${MYGUI_LIBRARY_NAMES} PATHS
  "/usr/lib"
  "/usr/local/lib"
  )
  
find_library ( MYGUI_LIBRARY_DBG
  ${MYGUI_LIBRARY_NAMES_DBG} PATHS
  "/usr/lib"
  "/usr/local/lib"
  )

make_library_set ( MYGUI_LIBRARY )

findpkg_finish ( "MYGUI" )
