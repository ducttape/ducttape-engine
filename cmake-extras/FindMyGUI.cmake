###############################################################################
# MyGUI
###############################################################################

include( "${CMAKE_MODULE_PATH}/FindPkgMacros.cmake" )

findpkg_begin ( "MYGUI" )

set ( MYGUI_LIBRARY_NAMES MyGUIEngine )
get_debug_names ( MYGUI_LIBRARY_NAMES )

find_path ( MYGUI_INCLUDE_DIR
  MyGUI.h PATHS
  ${MYGUI_ROOT}/MyGUIEngine/include
  "/usr/include/MYGUI"
  "/usr/local/include/MYGUI"
)

find_library ( MYGUI_LIBRARY_REL
  ${MYGUI_LIBRARY_NAMES} PATHS
  ${MYGUI_ROOT}/lib/Release
  ${MYGUI_ROOT}/lib/RelWithDebInfo
  ${MYGUI_ROOT}/lib/MinSizeRel
  "/usr/lib"
  "/usr/local/lib"
)

find_library ( MYGUI_LIBRARY_DBG
  ${MYGUI_LIBRARY_NAMES_DBG} PATHS
  ${MYGUI_ROOT}/lib/Debug
  "/usr/lib"
  "/usr/local/lib"
)

make_library_set ( MYGUI_LIBRARY )

findpkg_finish ( "MYGUI" )
