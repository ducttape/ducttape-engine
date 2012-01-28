
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
#include <Logic/FollowPathComponent.hpp>
#include <Logic/AgentComponent.h>

#include "Recast.h"
#include "DebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include "DetourCommon.h"
#include "DetourCrowd.h"

#include <QCoreApplication>
#include <QFile>

#include "BtOgreExtras.h"

class AgentComponent;

namespace dt {
    
class DUCTTAPE_API Crowd : public Manager {
    Q_OBJECT
    
    friend class NavigationManager; 
    
private:
    /**
     * Crowd are created through the Navigation Manager
     */
    Crowd(dtNavMesh* nav_mesh);
    ~Crowd();
    
public slots:
    void UpdateFrame(double simulation_frame_time);
        
public:
    virtual void Initialize();
    virtual void Deinitialize();
//     virtual int type() { return TOOL_CROWD; }
//     virtual void init(Sample* sample);
//     virtual void reset();
//     virtual void handleMenu();
//     virtual void handleClick(const float* s, const float* p, bool shift);
//     virtual void handleToggle();
//     virtual void handleStep();
//     virtual void handleUpdate(const float dt);
//     virtual void handleRender();
//     virtual void handleRenderOverlay(double* proj, double* model, int* view);
    
    /**
     * Get the detour crowd.
     * @return the detour crowd.
     */
    dtCrowd& GetCrowd() {return mCrowd;};
    
//     /**
//      * Get the navigation query.
//      * @return the navigation query.
//      */
//     dtNavMeshQuery* GetNavQuery() {return mNavQuery;};
    
    /**
     * Create a component that tracks an agent of the crowd.
     * User should add the returning component to a node.
     */
    AgentComponent* CreateAgentComponent(const Ogre::Vector3& position, const dtCrowdAgentParams& ap, const QString& name = "");
    
    /**
     * Create a default, valid configuration for an agent.
     * @return a default configuration.
     */
    dtCrowdAgentParams CreateDefaultConfig();
    
//     Sample* m_sample;
    unsigned char m_oldFlags;
    
    float m_targetPos[3];
    dtPolyRef m_targetRef;

    bool m_expandSelectedDebugDraw;
    bool m_showCorners;
    bool m_showCollisionSegments;
    bool m_showPath;
    bool m_showVO;
    bool m_showOpt;
    bool m_showNeis;

    bool m_expandDebugDraw;
    bool m_showLabels;
    bool m_showGrid;
    bool m_showNodes;
    bool m_showPerfGraph;
    
    bool m_expandOptions;
    bool m_anticipateTurns;
    bool m_optimizeVis;
    bool m_optimizeTopo;
    bool m_obstacleAvoidance;
    float m_obstacleAvoidanceType;
    bool m_separation;
    float m_separationWeight;
    
    bool m_run;

    dtCrowdAgentDebugInfo m_agentDebug;
    dtObstacleAvoidanceDebugData* m_vod;
    
    static const int AGENT_MAX_TRAIL = 64;
    static const int MAX_AGENTS = 128;
    static const int MAX_AGENT_RADIUS = 4;
    struct AgentTrail
    {
            float trail[AGENT_MAX_TRAIL*3];
            int htrail;
    };
    AgentTrail m_trails[MAX_AGENTS];
    
//     ValueHistory m_crowdTotalTime;
//     ValueHistory m_crowdSampleCount;
    
    
    enum ToolMode
    {
            TOOLMODE_CREATE,
            TOOLMODE_MOVE_TARGET,
            TOOLMODE_SELECT,
            TOOLMODE_TOGGLE_POLYS,
    };
    ToolMode m_mode;
    
    void updateAgentParams();
    void updateTick(const float dt);
    dtNavMesh* mNavMesh; //!< Detour navigation mesh.
    dtNavMeshQuery* mNavQuery; //!< The detour navigation mesh query.
    dtCrowd mCrowd; //!< Detour crowd.

}; 
    
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
    
    /**
     * Show the debug map.
     */
    void SetVisible(bool visible);
        
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
    NavigationManager();
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
     * @param Begin the begin point of the path.
     * @param End the end point of the path. 
     * @return a deque of vector (points of the path).
     */
    std::deque<Ogre::Vector3> FindPath(const Ogre::Vector3& Begin, const Ogre::Vector3& End);
    
    /** TODO rename method
     * Find the shortest path and append it to a 
     * FollowPath component.
     * @param Begin the begin point of the path.
     * @param End the end point of the path.
     * @param path the follow path to be modified.
     */
    void AddPathToComponent(const Ogre::Vector3& Begin, const Ogre::Vector3& End, dt::FollowPathComponent& path);
    
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
    
    /**
     * Create a default configuration.
     * @return a default configuration.
     */
    rcConfig CreateDefaultConfig();
    
    /**
     * Set a configuration.
     * @param config a configuration.
     */
    void SetConfig(const rcConfig& config);
    
    /**
     * Get the configuration in use.
     * @return the configuration in use.
     */
    rcConfig GetConfig();
    
    /**
     * Create the debug drawer.
     */
    void CreateDebugDrawer(Ogre::SceneManager* scene_mgr);
    
    /**
     * Show the debug map.
     */
    void ShowDebug(bool show);
    
    /**
     * Get the detour Navigation Query.
     * @return the detour Navigation Query.
     */
    dtNavMeshQuery* GetNavQuery() {return mNavQuery;};
    
    /**
     * Create a crowd.
     * @return a crowd.
     */
    Crowd* CreateCrowd();
    
public slots:
    void UpdateFrame(double simulation_frame_time);
    
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
    
    //! Agent
    float mAgentHeight;
    float mAgentMaxClimb;
    float mAgentRadius;
    
    std::deque<Crowd*> mCrowdList;
   
};


}

#endif
