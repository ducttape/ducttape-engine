
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
#include <Graphics/MeshComponent.hpp>
#include <Logic/FollowPathComponent.hpp>
#include <Logic/AgentComponent.hpp>

#include "Recast.h"
#include "DebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include "DetourCommon.h"
#include "DetourCrowd.h"

#include <QCoreApplication>
#include <QFile>

class AgentComponent;

namespace dt {
    
class DUCTTAPE_API Crowd : QObject {
    Q_OBJECT
    
friend class NavigationManager; 
    
private:
    /**
     * A Crowd is a set of navigation agent that share 
     * a common configuration.
     * Crowd are created through the Navigation Manager.
     */
    Crowd(dtNavMesh* nav_mesh);
    
    ~Crowd();
    
public slots:
    /**
     * Update the Crowd, move the agents depending on 
     * frame time.
     * @param simulation_frame_time the lenght of the frame.
     */
    void UpdateFrame(double simulation_frame_time);
        
public:
    
    /**
     * Create a component that tracks an agent of the crowd.
     * User may add the returning component to a node.
     * @param position the initial position of the agent.
     * @param params the parameters of the agent.
     * @param name the name of the node.
     */
    AgentComponent* CreateAgentComponent(const Ogre::Vector3& position, 
                                         const dtCrowdAgentParams& params, 
                                         const QString& name = "");
    
    /**
     * Create a component that tracks an agent of the crowd.
     * The new agent will have the global params of the crowd.
     * User may add the returning component to a node.
     * @param position the initial position of the agent.
     * @param name the name of the node.
     */
    AgentComponent* CreateAgentComponent(const Ogre::Vector3& position, const QString& name = "");
    
    /**
     * Create a default, valid configuration for an agent.
     * @return a default configuration.
     */
    dtCrowdAgentParams CreateDefaultConfig();
    
    /**
     * Get the detour crowd.
     * @return the detour crowd.
     */
    dtCrowd& GetCrowd() {return mCrowd;};
    
    /**
     * Set the same params for all the agents.
     * @param params the params for all the agents.
     */
    void SetCrowdParams(const dtCrowdAgentParams& params);

    dtCrowdAgentDebugInfo mAgentDebug; //!< Collects data of the agents.
    dtObstacleAvoidanceDebugData* mObstacleAvoidanceDebugData; //!< Collects data of the avoidance process.
    
    static const uint16_t AGENT_MAX_TRAIL = 64;
    static const uint16_t MAX_AGENTS = 128;
    static const uint16_t MAX_AGENT_RADIUS = 4;
    
    dtCrowd mCrowd; //!< Detour crowd.
    dtCrowdAgentParams mGlobalSharedParams; //!< Global params of the crowd
    float mTargetPos[3]; //! Position of the target.
    dtPolyRef mTargetRef; //! The polygon reference of the target.

}; 

/** 
 * Ogre Manual Object debug draw implementation.
 * This class will draw on the screen maps and paths.
 */
class DebugDraw : public duDebugDraw {
public:
    /**
     * This class will draw on the screen maps and paths.
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
    /**
     * Set the colour.
     */
    void _SetColour(unsigned int color);
    
    Ogre::ManualObject* mManualObject; //!< Manual object that draw lines and triangles.
    static const Ogre::String mMaterialName; //!< Name of the material used by the drawer.
};

/**
  * Manager class for the Navigation Engine.
  * In order to use the engine:
  * 1. Add the mesh to analyze with AddMesh().
  * 2. Build the navigation map with BuildMap().
  * 3. Request path with FindPath() or create crowd with CreateCrowd(). 
  */
class DUCTTAPE_API NavigationManager : public Manager {
    Q_OBJECT
    
public:
    /**
      * Manager class for the Navigation Engine.
      */
    NavigationManager();
    ~NavigationManager();
    
        
public slots:
    void UpdateFrame(double simulation_frame_time);
    
    /**
     * Remove the mesh from the navigation map.
     * Call BuilMap() in order to update the navigation map.
     * @param name The name of the mesh to be removed.
     */
    void RemoveMesh(const QString& name);
    
public:
    void Initialize();
    void Deinitialize();
    
    static NavigationManager* Get();
    
    /**
     * Create a crowd.
     * @return a crowd.
     */
    Crowd* CreateCrowd();
    
    /**
     * Create a default configuration.
     * @return a default configuration.
     */
    rcConfig CreateDefaultConfig();

    /**
     * Add a mesh to the navigation map.
     * @param mesh_component The MeshComponent to add.
     */
    void AddMesh(dt::MeshComponent* mesh);
    
    /**
     * Find the straight path from point Begin to point End.
     * @param Begin The begin point of the path.
     * @param End The end point of the path. 
     * @return a deque of vector (points of the path).
     */
    std::deque<Ogre::Vector3> FindPath(const Ogre::Vector3& Begin, const Ogre::Vector3& End);
    
    /**
     * Find the shortest path and append it to a 
     * FollowPath component.
     * @param Begin The begin point of the path.
     * @param End The end point of the path.
     * @param path The follow path to be modified.
     */
    void AddPathToComponent(const Ogre::Vector3& Begin, const Ogre::Vector3& End, dt::FollowPathComponent& path);
    
    /**
     * Build the navigation map.
     * Call this after the mesh are added.
     * @return true if the map was builded successfully.
     */
    bool BuildMap();
    
    /**
     * Update the NavQuery.
     */
    bool UpdateNavQuery();
    
    /**
     * Get the configuration in use.
     * @return the configuration in use.
     */
    rcConfig GetConfig();
    
    /**
     * Create the debug drawer.
     * @param scene_mgr The scene manager where the data will be drawn.
     */
    void InitDebugDrawer(Ogre::SceneManager* scene_mgr);
    
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
     * Reset the parameter to the default configuration
     */
    void ResetCommonSetting();
    
    /**
     * Set a configuration.
     * @param config a configuration.
     */
    void SetConfig(const rcConfig& config);
    
private:
    
    /**
     * Load all the added meshes.
     */
    void _LoadMeshes();
    
    /**
     * Add a mesh to the navigation map.
     * @param mesh the mesh to add.
     * @param position the absolute position of the mesh.
     * @param orient the absolute orientation of the mesh.
     * @param scale the absolute scale of the mesh.
     */
    void _AddInputMesh(Ogre::Mesh& mesh, const Ogre::Vector3& position,
                 const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
    
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
     * @return false on error.
     */
    bool _RastetizeMesh();
   
    /**
     * Filter the walkable surface.
     */
    void _FilterWalkableSurface();
   
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
     * Delete temponary data (vertices and indices)
     */
    void _DeleteTemporaryData();
    
    static const int32_t MAX_POLYS = 256;
    static const int32_t MAX_NODES = 2048;
    static const int16_t MAX_PATH_POINTS = 9000;
    static const int16_t MAX_PATH_POLYGONS = 1000;
    static const uint8_t SEARCH_DISTANCE_X = 2;
    static const uint8_t SEARCH_DISTANCE_Y = 4;
    static const uint8_t SEARCH_DISTANCE_Z = 2;
    static const uint8_t MAX_CONVEXVOL_PTS = 12;
   
    bool mKeepTempResult; //!< Wheter to keep temporary result for debugging purpose.
    float mTotalBuildTime; //!< The lenght of the build time of the navigation mesh in seconds.

    rcHeightfield* mSolid; //!< A dynamic heightfield representing obstructed space.
    rcCompactHeightfield* mCHF; //!< A compact, static heightfield representing unobstructed space.
    rcContourSet* mCSet; //!< A group of related contours.
    rcPolyMesh* mPMesh; //!< A polygon mesh suitable for use in building a navigation mesh.
    rcConfig mCFG; //!< A configuration to use when performing Recast builds.
    rcPolyMeshDetail* mDMesh; //!< Detailed height data associated with the polygons.
    rcContext mCTX; //!< Logging and performance analisys interface. 
    class dtNavMesh* mNavMesh; //!< A navigation mesh based on tiles of convex polygons
    class dtNavMeshQuery* mNavQuery; //!< The query that provides pathfinding.
    dtQueryFilter mFilter; //!< Filter for walkable area.
    DebugDraw* mDebugDraw; //!< The drawer for debbuging purpose.
    
    dtPolyRef mStartRef; //!< The reference id of the nearest polygon.
    dtPolyRef mEndRef; //!< The reference id of the nearest polygon.  
    unsigned char* mTriareas; //!< Array of triangles areas that will be walkable.
    int32_t mNStraightPath; //!< The number of points in the straight path.
    int32_t mNPolys; //!< The number of polygons in the straight path.
    dtPolyRef mPolys[MAX_PATH_POLYGONS*10]; //!< The Polygon of the straight path.
    unsigned char mStraightPathFlags[MAX_PATH_POINTS*10]; //!< Flags that describes points of the path.
    uint64_t mVerticesNumber; //!< Number of the vertices of the meshes to be rastetized.
    uint64_t mIndicesNumber; //!< Number of the indices of the meshes to be rastetized.
    float* mVertices; //!< Array to vertices of the meshes to be rastetized.
    int32_t* mIndices; //!< Array to indices of the meshes to be rastetized.
    Ogre::Vector3 mBeginPosition; //!< The start position of the path.
    Ogre::Vector3 mEndPosition; //!< The end position of the path.
    float mSearchDistance[3]; //!< The search distance (x, y, z).
    float mStraightPath[MAX_PATH_POINTS]; //!< Points of the straight path.
    dtPolyRef mStraightPathPolys[MAX_PATH_POLYGONS]; //!< Polygons of the straight path.
    
    float mAgentHeight; //! The agent height
    float mAgentMaxClimb; //! The agent max climb
    float mAgentRadius; //! The agent max radius
    
    std::deque<Crowd*> mCrowdList; //!< Deque containing all the crowds.
    std::map<QString, dt::MeshComponent*> mInputMeshList; //!< The mesh that will be analyzed.
    
    static const bool mMonotonePartitioning; //!< Wheter to use monotone partitioning.
    
};

}

#endif
