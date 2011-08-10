# Make BtOgre
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/btogre/include)
add_library(BtOgre STATIC ${CMAKE_CURRENT_SOURCE_DIR}/btogre/BtOgre.cpp)
#target_link_libraries(BtOgre ${BULLET_LIBRARIES} ${OGRE_LIBRARIES})

# Make OgreProcedural
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ogre-procedural/library/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ogre-procedural/library/src OgreProcedural_SOURCES)
add_library(OgreProcedural STATIC ${OgreProcedural_SOURCES})
set_target_properties(OgreProcedural PROPERTIES COMPILE_FLAGS "-DOgreProcedural_STATIC")
#target_link_libraries(OgreProcedural ${OGRE_LIBRARIES})

# Make ogre-paged
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ogre-paged/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ogre-paged/source ogre-paged_SOURCES)
add_library(ogre-paged STATIC ${ogre-paged_SOURCES})
#set_target_properties(ogre-paged PROPERTIES COMPILE_FLAGS "-DOgreProcedural_STATIC")
