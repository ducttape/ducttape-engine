# Make BtOgre
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/btogre/include)
add_library(BtOgre STATIC ${CMAKE_CURRENT_SOURCE_DIR}/btogre/BtOgre.cpp)
#target_link_libraries(BtOgre ${BULLET_LIBRARIES} ${OGRE_LIBRARIES})

# Make OgreProcedural
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ogre-procedural/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ogre-procedural/library/src OgreProcedural_SOURCES)
add_library(OgreProcedural STATIC ${OgreProcedural_SOURCES})
#target_link_libraries(OgreProcedural ${OGRE_LIBRARIES})
