
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_LOGIC_NAVIGATIONMANAGER
#define DUCTTAPE_ENGINE_LOGIC_NAVIGATIONMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>

// #include "OgreProceduralBoxGenerator.h"

#include "Recast.h"
#include "DebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include "DetourCommon.h"

#include <QCoreApplication>
#include <QFile>

#include "BtOgreExtras.h"

namespace dt {
    
enum SamplePolyAreas
{
        SAMPLE_POLYAREA_GROUND,
        SAMPLE_POLYAREA_WATER,
        SAMPLE_POLYAREA_ROAD,
        SAMPLE_POLYAREA_DOOR,
        SAMPLE_POLYAREA_GRASS,
        SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
        SAMPLE_POLYFLAGS_WALK           = 0x01,         // Ability to walk (ground, grass, road)
        SAMPLE_POLYFLAGS_SWIM           = 0x02,         // Ability to swim (water).
        SAMPLE_POLYFLAGS_DOOR           = 0x04,         // Ability to move through doors.
        SAMPLE_POLYFLAGS_JUMP           = 0x08,         // Ability to jump.
        SAMPLE_POLYFLAGS_DISABLED       = 0x10,         // Disabled polygon
        SAMPLE_POLYFLAGS_ALL            = 0xffff        // All abilities.
};

/** 
 * Ogre Manual Object debug draw implementation.
 * This class will draw on the screen maps or paths.
 */
class DebugDraw : public duDebugDraw
{
public:
    /**
     * This class will draw on the screen maps or paths.
     * @param scene_mgr The scene manager where the data will be drawn.
     */
        DebugDraw(Ogre::SceneManager* scene_mgr);
        
        virtual void depthMask(bool state);
        virtual void texture(bool state);
        virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
        virtual void vertex(const float* pos, unsigned int color);
        virtual void vertex(const float x, const float y, const float z, unsigned int color);
        virtual void vertex(const float* pos, unsigned int color, const float* uv);
        virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
        virtual void end();
        
private:
    Ogre::ManualObject* mManualObject; //!< Manual object that draw lines and triangles.
};

/**
  * Manager class for the Navigation Engine.
  * In order to use the engine:
  * 1. Add the mesh to analyze with AddMesh().
  * 2. Build the navigation map with BuildMap().
  * 3. Request path with FindPath().
  */
class DUCTTAPE_API NavigationManager : public Manager {
    Q_OBJECT
    
public:
    /**
      * Manager class for the Navigation Engine.
      */
    NavigationManager(Ogre::SceneManager* scene_mgr);
    ~NavigationManager();

    void Initialize();
    void Deinitialize();
    static NavigationManager* Get();

//     bool AddNavigation(QString script, QString name);
    
    /**
     * Add a mesh to the navigation map.
     * @param mesh the mesh to add.
     * @param position the position of the mesh.
     * @param orient the orientation of the mesh.
     * @param scale the scale of the mesh.
     */
    void AddMesh(Ogre::Mesh& mesh, const Ogre::Vector3 &position,
                 const Ogre::Quaternion &orient, const Ogre::Vector3 &scale );
    
    /**
     * Find the straight path from point Begin to point End.
     * @return a deque of vector (points of the path).
     */
    std::deque<Ogre::Vector3> FindPath(const Ogre::Vector3& Begin, const Ogre::Vector3& End);
    
    /**
     * Build the navigation map.
     * Call this after the mesh are added.
     * @return true if the map was builded successfully.
     */
    bool BuildMap();
    
    /**
     * Reset the parameter to the default configuration
     */
    void ResetCommonSetting();
    
    /**
     * Update the NavQuery.
     */
    bool UpdateNavQuery();
    
    bool mKeepTempResult;
    float mTotalBuildTime;

    unsigned char* mTriareas;
    rcHeightfield* mSolid;
    rcCompactHeightfield* mCHF;
    rcContourSet* mCSet;
    rcPolyMesh* mPMesh;
    rcConfig mCFG; 
    rcPolyMeshDetail* mDMesh;
    rcContext mCTX; //TODO Inherit another class from rcContext
    class dtNavMesh* mNavMesh;
    class dtNavMeshQuery* mNavQuery;
    class dtCrowd* mCrowd;
    
    // Filter for walkable area.
    dtQueryFilter mFilter; 
    // The reference id of the nearest polygon.
    dtPolyRef mStartRef;
    // The reference id of the nearest polygon.
    dtPolyRef mEndRef;
    
    // The number of points in the straight path
    int mNStraightPath;
    int mNPolys;

    float mHitPos[3];
    float mHitNormal[3];
    dtPolyRef mPolys[10000];
    unsigned char mStraightPathFlags[10000];
    
    
private:
    /**
     * @param mesh the mesh to add.
     * @param vertex_count The number of the vertex.
     * @param vertices The vertices data (x, y, z) * vertex_count.
     * @param index_count The number of the indices.
     * @param indices The indices data, (index a, index b, index c) * triangle_count.
     * @param position the position of the mesh.
     * @param orient the orientation of the mesh.
     * @param scale the scale of the mesh.
     */
   void _GetMeshVertices(const Ogre::Mesh* const mesh,
                        size_t &vertex_count,
                        float*& vertices,
                        size_t &index_count,
                        int32_t* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale);
   /**
    * Rastetize the navigation mesh.
    */
   bool _RastetizeMesh();
   
   /**
    * Filter the walkable surface.
    */
   bool _FilterWalkableSurface();
   
   /**
    *  Partition walkable surface to simple regions.
    */
   bool _PartitionSurfaceToRegions();
   
   /**
    *  Trace and simplify region contours.
    */
   bool _TraceAndSimplifyRegion();
   
   /**
    * Build polygons mesh from contours.
    */
   bool _BuildPolygonsMesh();
   
   /**
    * Create detail mesh which allows to access approximate height on each polygon.
    */
   bool _BuildDetailMesh();
   
   /**
    * Create Detour data from Recast poly mesh.
    */
   bool _CreateDetourData();
   
   /**
    * Convert an array of 3 floats to a Ogre::Vector3
    * @param vector an array of 3 floats.
    * @return an ogre vector.
    */
   Ogre::Vector3 _FloatsToOgreVector3(float* vector);
    /**
    * Convert an Ogre::Vector3 to an array of 3 floats.
    * @param ogre_vector an ogre vector.
    * @param float_vector an array of 3 floats. warning: the float memory must be writable.
    */
   void _OgreVector3ToFloats(const Ogre::Vector3& ogre_vector, float* float_vector);
   
   uint64_t mVerticesNumber; //!< number of the vertices of the meshes to be rastetized.
   uint64_t mIndicesNumber; //!< number of the indices of the meshes to be rastetized.
   float* mVertices; //!< array to vertices of the meshes to be rastetized.
   int32_t* mIndices; //!< array to indices of the meshes to be rastetized.
   
   Ogre::Vector3 mBeginPosition; //!< The start position of the path.
   Ogre::Vector3 mEndPosition; //!< The end position of the path.
   
   float mSearchDistance[3]; 
   static const int MAX_POLYS = 256;
   float mStraightPath[9000];
   dtPolyRef mStraightPathPolys[10000];
     
   DebugDraw* mDebugDraw; //!< The drawer for debbuging purpose.
   std::ofstream log; //!< output stream for printing Detour debug data.
   
};

}

#endif
