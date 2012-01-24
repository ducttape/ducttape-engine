
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
// #include <Logic/NavigationComponent.hpp>

#include <QCoreApplication>
#include <QFile>

#include "OgreProceduralBoxGenerator.h"

#include "Recast.h"
#include "DebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include "DetourCommon.h"

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

/// OpenGL debug draw implementation.
class DebugDrawGL : public duDebugDraw
{
public:
        DebugDrawGL(Ogre::SceneManager* scene_mgr);
        virtual void depthMask(bool state);
        virtual void texture(bool state);
        virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
        virtual void vertex(const float* pos, unsigned int color);
        virtual void vertex(const float x, const float y, const float z, unsigned int color);
        virtual void vertex(const float* pos, unsigned int color, const float* uv);
        virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
        virtual void end();
        
//     btIDebugDraw mDebugDraw;
private:
    Ogre::ManualObject* mManualObject;
};

/**
  * Manager class for the Navigationing Engine.
  */
class DUCTTAPE_API NavigationManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    NavigationManager(Ogre::SceneManager* scene_mgr);

    void Initialize();
    void Deinitialize();
    static NavigationManager* Get();

    bool AddNavigation(QString script, QString name);
    
    /**
     * Add a mesh to the navigation map.
     */
    void AddMesh(Ogre::Mesh& mesh, const Ogre::Vector3 &position,
                 const Ogre::Quaternion &orient, const Ogre::Vector3 &scale );
    
    /**
     * Find the straight path from point Begin to point End.
     * @return a deque of vector (points).
     */
    std::deque<Ogre::Vector3> FindPath(const Ogre::Vector3& Begin, const Ogre::Vector3& End);
    
    /**
     * Build the navigation map.
     * Call this after the mesh where added.
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
    
    bool m_keepInterResults;
    float m_totalBuildTimeMs;

    unsigned char* m_triareas;
    rcHeightfield* m_solid;
    rcCompactHeightfield* m_chf;
    rcContourSet* m_cset;
    rcPolyMesh* m_pmesh;
    rcConfig m_cfg; 
    rcPolyMeshDetail* m_dmesh;
    rcContext m_ctx; //TODO Inherit another class from rcContext
    class dtNavMesh* m_navMesh;
    class dtNavMeshQuery* m_navQuery;
    class dtCrowd* m_crowd;
    
    // Filter for walkable area.
    dtQueryFilter mFilter; 
    // The reference id of the nearest polygon.
    dtPolyRef mStartRef;
    // The reference id of the nearest polygon.
    dtPolyRef mEndRef;
    
    // The number of points in the straight path
    int m_nstraightPath;
    int m_npolys;

        float m_hitPos[3];
        float m_hitNormal[3];
                dtPolyRef m_polys[10000];
    unsigned char m_straightPathFlags[10000];
    
    
private:
    /**
     * @param vertex_count The number of the vertex.
     * @param vertices The vertices data (x, y, z) * vertex_count.
     * @param index_count The number of the indices.
     * @param indices The indices data, (index a, index b, index c) * triangle_count.
     */
   void _GetMeshVertices(const Ogre::Mesh* const mesh,
                        size_t &vertex_count,
                        float*& vertices,
                        size_t &index_count,
                        int32_t* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale);
   void _RastetizeMesh();
   
   Ogre::Vector3 _FloatsToOgreVector3(float* vector);
   void _OgreVector3ToFloats(const Ogre::Vector3& ogre_vector, float* float_vector);
   
   uint64_t mVerticesNumber; //!< number of the vertices of the meshes to be rastetized.
   uint64_t mIndicesNumber; //!< number of the indices of the meshes to be rastetized.
   float* mVertices; //!< array to vertices of the meshes to be rastetized.
   int32_t* mIndices; //!< array to indices of the meshes to be rastetized.
   
   Ogre::Vector3 mBeginPosition;
   Ogre::Vector3 mEndPosition;
   float mSearchDistance[3]; 
   
        static const int MAX_POLYS = 256;
        
    float mStraightPath[9000];
     dtPolyRef mStraightPathPolys[10000];
     
     DebugDrawGL* mDebugDraw;
    
};

}

#endif
