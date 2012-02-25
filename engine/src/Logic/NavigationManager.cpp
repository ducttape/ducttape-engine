
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/NavigationManager.hpp>
#include <Core/Root.hpp>
#include <Gui/GuiManager.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>
#include <Input/MouseState.hpp>
#include <Input/KeyboardState.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Core/ResourceManager.hpp>

#include <QString>

#include "Recast.h"
#include "RecastDebugDraw.h"
#include "RecastDump.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include <DetourNode.h>
#include <Scene/Node.hpp>

namespace dt {
    
const bool NavigationManager::mMonotonePartitioning = true;

Crowd::Crowd(dtNavMesh* nav_mesh) {
    mObstacleAvoidanceDebugData = dtAllocObstacleAvoidanceDebugData();
    mObstacleAvoidanceDebugData->init(2048);
    
    memset(&mAgentDebug, 0, sizeof(mAgentDebug));
    mAgentDebug.idx = -1;
    mAgentDebug.vod = mObstacleAvoidanceDebugData;
    
    mGlobalSharedParams = CreateDefaultConfig();
    
    if(!nav_mesh) {
        dt::Logger::Get().Error("The navigation mesh is not valid");
    } else {
        mCrowd.init(MAX_AGENTS, MAX_AGENT_RADIUS, nav_mesh);

        // Make polygons with 'disabled' flag invalid.
        mCrowd.getEditableFilter()->setExcludeFlags(0);
        
        // Setup local avoidance params to different qualities.
        dtObstacleAvoidanceParams params;
        // Use mostly default settings, copy from dtCrowd.
        memcpy(&params, mCrowd.getObstacleAvoidanceParams(0), sizeof(dtObstacleAvoidanceParams));
        
        // Low (11)
        params.velBias = 0.5f;
        params.adaptiveDivs = 5;
        params.adaptiveRings = 2;
        params.adaptiveDepth = 1;
        mCrowd.setObstacleAvoidanceParams(0, &params);

        // Medium (22)
        params.velBias = 0.5f;
        params.adaptiveDivs = 5;
        params.adaptiveRings = 2;
        params.adaptiveDepth = 2;
        mCrowd.setObstacleAvoidanceParams(1, &params);
        
        // Good (45)
        params.velBias = 0.5f;
        params.adaptiveDivs = 7;
        params.adaptiveRings = 2;
        params.adaptiveDepth = 3;
        mCrowd.setObstacleAvoidanceParams(2, &params);
        
        // High (66)
        params.velBias = 0.5f;
        params.adaptiveDivs = 7;
        params.adaptiveRings = 3;
        params.adaptiveDepth = 3;
        
        mCrowd.setObstacleAvoidanceParams(3, &params);
    }
}

Crowd::~Crowd() {
    dtFreeObstacleAvoidanceDebugData(mObstacleAvoidanceDebugData);
}

void Crowd::UpdateFrame(double simulation_frame_time) {
    mCrowd.update(simulation_frame_time, &mAgentDebug);
}    
    
AgentComponent* Crowd::CreateAgentComponent(const Ogre::Vector3& position, const dtCrowdAgentParams& ap, const QString& name) {
    float pos[3];
    dt::Utils::OgreVector3ToFloats(position, pos);
    
    int index = mCrowd.addAgent(pos, &ap);
    if(index != -1) {
        if(mTargetRef)
                mCrowd.requestMoveTarget(index, mTargetRef, mTargetPos);
    }

    AgentComponent* agent_component = new AgentComponent(this, index, name);
    return agent_component;
}

AgentComponent* Crowd::CreateAgentComponent(const Ogre::Vector3& position, const QString& name) {
    CreateAgentComponent(position, mGlobalSharedParams, name);
}

dtCrowdAgentParams Crowd::CreateDefaultConfig() {
    dtCrowdAgentParams ap;
    memset(&ap, 0, sizeof(ap));
    ap.radius = 1;
    ap.height = 1;
    ap.maxAcceleration = 10.0f;
    ap.maxSpeed = 4.5f;
    ap.collisionQueryRange = ap.radius * 12.0f;
    ap.pathOptimizationRange = ap.radius * 30.0f;
    ap.updateFlags = 0; 
    ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
    ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;
    ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
    ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
    ap.updateFlags |= DT_CROWD_SEPARATION;
    ap.obstacleAvoidanceType = 3;
    ap.separationWeight = 2.0f;
    return ap;
}

void Crowd::SetCrowdParams(const dtCrowdAgentParams& params) {
    for(uint32_t i = 0; i < mCrowd.getAgentCount(); ++i) {
        const dtCrowdAgent* agent = mCrowd.getAgent(i);
        if(agent->active) {
            mCrowd.updateAgentParameters(i, &params);
        }
    }       
}

NavigationManager::NavigationManager() : 
mVerticesNumber(0),
mIndicesNumber(0),
mVertices(nullptr),
mIndices(nullptr),
mSolid(nullptr),
mCHF(nullptr),
mCSet(nullptr),
mDMesh(nullptr),
mNavMesh(nullptr),
mNavQuery(nullptr),
mDebugDraw(nullptr),
mTriareas(nullptr),
mKeepTempResult(false) {}

NavigationManager::~NavigationManager() {
    delete [] mTriareas;
    mTriareas = 0;
    rcFreeHeightField(mSolid);
    mSolid = 0;
    rcFreeCompactHeightfield(mCHF);
    mCHF = 0;
    rcFreeContourSet(mCSet);
    mCSet = 0;
    rcFreePolyMesh(mPMesh);
    mPMesh = 0;
    rcFreePolyMeshDetail(mDMesh);
    mDMesh = 0;
    dtFreeNavMesh(mNavMesh);
    mNavMesh = 0;
}

void NavigationManager::UpdateFrame(double simulation_frame_time) {
    for(std::deque<Crowd*>::iterator it = mCrowdList.begin(); it < mCrowdList.end(); ++it) {
        (*it)->UpdateFrame(simulation_frame_time);
    }
}

void NavigationManager::Initialize() {
    mFilter.setIncludeFlags(~0);
    mFilter.setExcludeFlags(0);
    
    mNavQuery = dtAllocNavMeshQuery();
    
    mCFG = CreateDefaultConfig();
}

void NavigationManager::Deinitialize() {}

NavigationManager* NavigationManager::Get() {
    return Root::GetInstance().GetNavigationManager();
}

Crowd* NavigationManager::CreateCrowd() {
    Crowd* crowd = new Crowd(mNavMesh);
    mCrowdList.push_back(crowd);
    return crowd;
}

rcConfig NavigationManager::CreateDefaultConfig() {
    rcConfig config;
    memset(&config, 0, sizeof(config));
    config.cs = 0.3f;
    config.ch = 0.2f;
    config.walkableSlopeAngle = 45.0f;
    config.walkableHeight = (int32_t)ceilf(2.0f / config.ch);
    config.walkableClimb = (int32_t)floorf(0.9f / config.ch);
    config.walkableRadius =  (int32_t)ceilf(0.6f / config.cs);
    config.maxEdgeLen = (int32_t)(12.0f / 0.3f); 12;
    config.maxSimplificationError = 1.3f;
    config.minRegionArea = (int32_t)rcSqr(8);   
    config.mergeRegionArea = (int32_t)rcSqr(20);
    config.maxVertsPerPoly = (int32_t)6.0f;
    config.detailSampleDist = 6.0f < 0.9f ? 0 : 0.3f *  6.0f;
    config.detailSampleMaxError = 0.2f * 1.0f;
    return config;
}

void NavigationManager::AddMesh(dt::MeshComponent* mesh) {
    const QString name(mesh->GetName());
    if(mInputMeshList.count(name)) {
        return;
    }
    mInputMeshList.insert(std::make_pair(name, mesh));
    connect(mesh, SIGNAL(DeletingMesh(QString)), this, SLOT(RemoveMesh(QString)));
}

void NavigationManager::RemoveMesh(const QString& name) {
    mInputMeshList.erase(name);
}

void NavigationManager::InitDebugDrawer(Ogre::SceneManager* scene_mgr) {
     mDebugDraw = new DebugDraw(scene_mgr);
     mDebugDraw->SetVisible(true);
}

void NavigationManager::ShowDebug(bool show) {
    if(mDebugDraw) { 
        mDebugDraw->SetVisible(show);
    } else {
        dt::Logger::Get().Error("No navigation debug drawer created, create it with CreateDebugDrawer()");
    }
}

void NavigationManager::ResetCommonSetting() {
    mCFG = CreateDefaultConfig();
}

void NavigationManager::SetConfig(const rcConfig& config) {
    mCFG = config;
    
    // Some data must be modified, the original provided by the user
    // is stored in this class private data.
    mAgentHeight = mCFG.walkableHeight;
    mCFG.walkableHeight = (int32_t)ceilf(mAgentHeight / config.ch);
    mAgentMaxClimb = mCFG.walkableClimb;
    mCFG.walkableClimb = (int32_t)floorf(mAgentMaxClimb / config.ch);
    mAgentRadius = mCFG.walkableRadius;
    mCFG.walkableRadius = (int32_t)ceilf(mAgentRadius / config.cs);
}

rcConfig NavigationManager::GetConfig() {
    return mCFG;
}

bool NavigationManager::_RastetizeMesh() {   
    float bmin[3];
    float bmax[3];
    
    rcCalcBounds(mVertices, mVerticesNumber, bmin, bmax);
    uint32_t ntris = mIndicesNumber * 3;
    
    //
    // Step 1. Initialize build config.
    //
    
    ResetCommonSetting();
    
    mCTX.enableLog(true);

    // Set the area where the navigation will be build.
    // Here the bounds of the input mesh are used, but the
    // area could be specified by an user defined box, etc.
    rcVcopy(mCFG.bmin, bmin);
    rcVcopy(mCFG.bmax, bmax);
    rcCalcGridSize(mCFG.bmin, mCFG.bmax, mCFG.cs, &mCFG.width, &mCFG.height);
    
    //
    // Step 2. Rasterize input polygon soup.
    //
        
    // Allocate voxel heightfield where we rasterize our input data to.
    mSolid = rcAllocHeightfield();
    if(!mSolid) {
        dt::Logger::Get().Debug( "Cannot alloc an height field");
        return false;
    }
    if(!rcCreateHeightfield(&mCTX, *mSolid, mCFG.width, mCFG.height, 
                                mCFG.bmin, mCFG.bmax, mCFG.cs, mCFG.ch)) {
        dt::Logger::Get().Debug( "Cannot create an height field ");
        return false;
    }
    
    // Allocate array that can hold triangle area types.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.

    mTriareas = new unsigned char[ntris];
    if(!mTriareas) {
        dt::Logger::Get().Debug( "Cannot alloc array for triangle areas");
        return false;
    }
    
    int32_t numTris = mIndicesNumber / 3;
    
    // Find triangles which are walkable based on their slope and rasterize them.
    // If your input data is multiple meshes, you can transform them here, calculate
    // the are type for each of the meshes and rasterize them.
    memset(mTriareas, 0, ntris*sizeof(unsigned char));
    rcMarkWalkableTriangles(&mCTX, mCFG.walkableSlopeAngle, mVertices, mVerticesNumber, mIndices, numTris, mTriareas);
    rcRasterizeTriangles(&mCTX, mVertices, mVerticesNumber, mIndices, mTriareas, numTris, *mSolid, mCFG.walkableClimb);

    if(!mKeepTempResult) {
        delete [] mTriareas;
        mTriareas = 0;
    }
        
    return true;
}

void NavigationManager::_FilterWalkableSurface()
{
    // Once all geoemtry is rasterized, we do initial pass of filtering to
    // remove unwanted overhangs caused by the conservative rasterization
    // as well as filter spans where the character cannot possibly stand.
    rcFilterLowHangingWalkableObstacles(&mCTX, mCFG.walkableClimb, *mSolid);
    rcFilterLedgeSpans(&mCTX, mCFG.walkableHeight, mCFG.walkableClimb, *mSolid);
    rcFilterWalkableLowHeightSpans(&mCTX, mCFG.walkableHeight, *mSolid);
}

bool NavigationManager::_PartitionSurfaceToRegions()
{
    //
    // Step 4. Partition walkable surface to simple regions.
    //

    // Compact the heightfield so that it is faster to handle from now on.
    // This will result more cache coherent data as well as the neighbours
    // between walkable cells will be calculated.
    mCHF = rcAllocCompactHeightfield();
    if(!mCHF) {
        dt::Logger::Get().Debug( "Cannot alloc a compact heightfield");
        return false;
    }
    if(!rcBuildCompactHeightfield(&mCTX, mCFG.walkableHeight, mCFG.walkableClimb, *mSolid, *mCHF)) {
        dt::Logger::Get().Debug( "Cannot build a compact heightfield");
        return false;
    }
    
    if(!mKeepTempResult) {
        rcFreeHeightField(mSolid);
        mSolid = 0;
    }
            
    // Erode the walkable area by agent radius.
    if(!rcErodeWalkableArea(&mCTX, mCFG.walkableRadius, *mCHF)) {
        dt::Logger::Get().Debug( "ErodeWalkableArea() failed");
        return false;
    }
    
    if(mMonotonePartitioning) {
        // Partition the walkable surface into simple regions without holes.
        // Monotone partitioning does not need distancefield.
        if(!rcBuildRegionsMonotone(&mCTX, *mCHF, 0, mCFG.minRegionArea, mCFG.mergeRegionArea)) {
                dt::Logger::Get().Debug("Could not build regions.");
                return false;
        }
    } else {
        // Prepare for region partitioning, by calculating distance field along the walkable surface.
        if(!rcBuildDistanceField(&mCTX, *mCHF)) {
                dt::Logger::Get().Debug("Could not build distance field.");
                return false;
        }

        // Partition the walkable surface into simple regions without holes.
        if(!rcBuildRegions(&mCTX, *mCHF, 0, mCFG.minRegionArea, mCFG.mergeRegionArea)) {
                dt::Logger::Get().Debug("Could not build regions.");
                return false;
        }
    }
    return true;
}

bool NavigationManager::_TraceAndSimplifyRegion()
{
    // Create contours.
    mCSet = rcAllocContourSet();
    if(!mCSet) {
        dt::Logger::Get().Debug("Cannot alloc a contour set.");
        return false;
    }
    if(!rcBuildContours(&mCTX, *mCHF, mCFG.maxSimplificationError, mCFG.maxEdgeLen, *mCSet)) {
        dt::Logger::Get().Debug("Could not build the contour set.");
        return false;
    }
    return true;
}

bool NavigationManager::_BuildPolygonsMesh() {
    // Build polygon navmesh from the contours.
    mPMesh = rcAllocPolyMesh();
    if(!mPMesh) {
        dt::Logger::Get().Debug("Cannot alloc a poly mesh");
        return false;
    }
    if(!rcBuildPolyMesh(&mCTX, *mCSet, mCFG.maxVertsPerPoly, *mPMesh)) {
        dt::Logger::Get().Debug("Could not build the poly mesh");
        return false;
    }
    return true;
}


bool NavigationManager::_BuildDetailMesh() {
    mDMesh = rcAllocPolyMeshDetail();
    if(!mDMesh) {
        mCTX.log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
        return false;
    }

    if(!rcBuildPolyMeshDetail(&mCTX, *mPMesh, *mCHF, mCFG.detailSampleDist, mCFG.detailSampleMaxError, *mDMesh)) {
        mCTX.log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
        return false;
    }
    if(!mKeepTempResult) {
        rcFreeCompactHeightfield(mCHF);
        mCHF = 0;
        rcFreeContourSet(mCSet);
        mCSet = 0;
    }

    // At this point the navigation mesh data is ready, you can access it from m_pmesh.
    // See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
    return true;
}

bool NavigationManager::_CreateDetourData() {
    // The GUI may allow more max points per polygon than Detour can handle.
    // Only build the detour navmesh if we do not exceed the limit.
    if(mCFG.maxVertsPerPoly <= DT_VERTS_PER_POLYGON) {
        unsigned char* navData = 0;
        int navDataSize = 0;

        // Update poly flags from areas.
        for(uint32_t i = 0; i < mPMesh->npolys; ++i) {
            if(mPMesh->areas[i] == RC_WALKABLE_AREA) {
                //Set all the areas valide for the path.
                //Here it's possible to add flag for each area (like water, grass etc.)
                    mPMesh->areas[i] = 0;
                    mPMesh->flags[i] = 1;           
            }
        }

        dtNavMeshCreateParams params;
        memset(&params, 0, sizeof(params));
        params.verts = mPMesh->verts;
        params.vertCount = mPMesh->nverts;
        params.polys = mPMesh->polys;
        params.polyAreas = mPMesh->areas;
        params.polyFlags = mPMesh->flags;
        params.polyCount = mPMesh->npolys;
        params.nvp = mPMesh->nvp;
        params.detailMeshes = mDMesh->meshes;
        params.detailVerts = mDMesh->verts;
        params.detailVertsCount = mDMesh->nverts;
        params.detailTris = mDMesh->tris;
        params.detailTriCount = mDMesh->ntris;
        params.offMeshConCount = 0;
        params.walkableHeight = mAgentHeight;
        params.walkableRadius = mAgentRadius;
        params.walkableClimb = mAgentMaxClimb;
        rcVcopy(params.bmin, mPMesh->bmin);
        rcVcopy(params.bmax, mPMesh->bmax);
        params.cs = mCFG.cs;
        params.ch = mCFG.ch;
        params.buildBvTree = true;
        
        if(!dtCreateNavMeshData(&params, &navData, &navDataSize)) {
            dt::Logger::Get().Debug("Could not create Detour navmesh data.");
            return false;
        }
        
        mNavMesh = dtAllocNavMesh();
        if(!mNavMesh) {
            dtFree(navData);
            dt::Logger::Get().Debug("Could not create Detour navmesh");
            return false;
        }
        
        dtStatus status;
        
        status = mNavMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
        if(dtStatusFailed(status)) {
                dtFree(navData);
            dt::Logger::Get().Debug("Could not init Detour navmesh");
            return false;
        }
        
        status = mNavQuery->init(mNavMesh, MAX_NODES);
        if(dtStatusFailed(status)) {
            dt::Logger::Get().Debug("Could not init Detour navmesh query");
            return false;
        }
    }
    
    mCTX.stopTimer(RC_TIMER_TOTAL);

    // Show performance stats.
    duLogBuildTimes(mCTX, mCTX.getAccumulatedTime(RC_TIMER_TOTAL));
    dt::Logger::Get().Debug("Polymesh: " + dt::Utils::ToString(mVerticesNumber) + " vertices and " + dt::Utils::ToString(mIndicesNumber/3) + " triangles");
    
    mTotalBuildTime = mCTX.getAccumulatedTime(RC_TIMER_TOTAL)/1000.0f;
    
    duDebugDrawNavMeshPolysWithFlags(mDebugDraw, *mNavMesh, ~0, duRGBA(0,0,0,128));
    
    return true;
}

void NavigationManager::_DeleteTemporaryData() {
    delete[] mVertices;
    delete[] mIndices;
    mVertices = 0;
    mIndices = 0;
    mVerticesNumber = 0;
    mIndicesNumber = 0;
}

bool NavigationManager::BuildMap() {
    
    _LoadMeshes();
    
    if(!_RastetizeMesh()) {
        return false;
    }
    
    _FilterWalkableSurface();

    if(!_PartitionSurfaceToRegions()) {
        return false;
    }
   
    if(!_TraceAndSimplifyRegion()) {
        return false;
    }
    
    if(!_BuildPolygonsMesh()) {
        return false;
    }
    
    if(!_BuildDetailMesh()) {
        return false;
    }
    
    if(!_CreateDetourData()) {
        return false;
    }
    
    _DeleteTemporaryData();
    
    return true;
}

bool NavigationManager::UpdateNavQuery() {
    mSearchDistance[0] = SEARCH_DISTANCE_X;
    mSearchDistance[1] = SEARCH_DISTANCE_Y;
    mSearchDistance[2] = SEARCH_DISTANCE_Z;
    
    float start_pos[3];
    dt::Utils::OgreVector3ToFloats(mBeginPosition, start_pos);
    mNavQuery->findNearestPoly(start_pos, mSearchDistance, &mFilter, &mStartRef, 0);
    
    float end_pos[3];
    dt::Utils::OgreVector3ToFloats(mEndPosition, end_pos);
    mNavQuery->findNearestPoly(end_pos, mSearchDistance, &mFilter, &mEndRef, 0);
    
    
    dtStatus status = mNavQuery->findPath(mStartRef, mEndRef, start_pos, end_pos, &mFilter, mPolys, &mNPolys, MAX_POLYS);
    if(status & (DT_INVALID_PARAM | DT_FAILURE)) {
        dt::Logger::Get().Debug("findpath() failed, invalid param or other problem");
    }
    mNStraightPath = 0;
    if(mNPolys) {
            // In case of partial path, make sure the end point is clamped to the last polygon.
            float epos[3];
            dtVcopy(epos, end_pos);
            if(mPolys[mNPolys-1] != mEndRef) {
                    mNavQuery->closestPointOnPoly(mPolys[mNPolys-1], end_pos, epos);
            }
            
            mNavQuery->findStraightPath(start_pos, epos, mPolys, mNPolys,
                                              mStraightPath, mStraightPathFlags,
                                                  mStraightPathPolys, &mNStraightPath, MAX_POLYS);
    }
            
}

std::deque<Ogre::Vector3> NavigationManager::FindPath(const Ogre::Vector3& begin, const Ogre::Vector3& end) {
    mBeginPosition = begin;
    mEndPosition = end;
    
    // Create the mNStraightPath
    UpdateNavQuery();
    
    std::deque<Ogre::Vector3> path;
    // Float triplets to Ogre vector3.
    for(uint32_t i = 0; i < mNStraightPath; ++i) {
        path.push_back(dt::Utils::FloatsToOgreVector3(&mStraightPath[i*3]));
    }
    
    return path;
}

void NavigationManager::AddPathToComponent(const Ogre::Vector3& begin, const Ogre::Vector3& end, FollowPathComponent& path) {
    mBeginPosition = begin;
    mEndPosition = end;
    
    // Create the mNStraightPath
    UpdateNavQuery();
    
    // Float triplets to Ogre vector3.
    for(uint32_t i = 0; i < mNStraightPath; ++i) {
        path.AddPoint(dt::Utils::FloatsToOgreVector3(&mStraightPath[i*3]));
    }
}

void NavigationManager::_LoadMeshes() {
    for(std::map<QString, dt::MeshComponent*>::iterator it = mInputMeshList.begin(); 
                                            it != mInputMeshList.end(); ++it) {
        dt::MeshComponent* mesh = it->second;
        Ogre::Mesh* ogre_mesh = mesh->GetOgreEntity()->getMesh().get();
        dt::Node* mesh_node = mesh->GetNode();
        _AddInputMesh(*ogre_mesh,
                      mesh_node->GetPosition() ,
                      mesh_node->GetRotation(),
                      mesh_node->GetScale());
    }
}

void NavigationManager::_AddInputMesh(Ogre::Mesh& mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale) {
    uint64_t new_vertex_count(0), new_index_count(0);
    float* new_vertices = nullptr;
    int32_t* new_indices = nullptr;
    
    _GetMeshVertices(&mesh, new_vertex_count, new_vertices, new_index_count, new_indices, 
                     position, orient, scale);
    
    // Copy all vertices, new and old in a new array, erase the old array.
    float* vertices = new float[mVerticesNumber * 3 + new_vertex_count * 3];    
    memcpy(vertices, mVertices, sizeof(float) * mVerticesNumber * 3);
    memcpy(&vertices[mVerticesNumber*3], new_vertices, sizeof(float) * new_vertex_count * 3); //TODO: check limit
    delete[] mVertices;
    delete[] new_vertices;

    // Copy all indices, new and old in a new array, add offset, erase the old array.
    int32_t* indices = new int32_t[static_cast<size_t>(mIndicesNumber) + new_index_count];
    memcpy(indices, mIndices, sizeof(int32_t) * mIndicesNumber);
    for(uint32_t i = 0; i<new_index_count; ++i) {
        // Copy the new indices, but add an offset because there are the old vertices too.
        indices[mIndicesNumber+i] = mVerticesNumber + new_indices[i];
    }
    delete[] mIndices;
    delete[] new_indices;

    mVertices = vertices;
    mVerticesNumber += new_vertex_count;
    mIndices = indices;
    mIndicesNumber += new_index_count;    
}

void NavigationManager::_GetMeshVertices(const Ogre::Mesh* const mesh, uint64_t& vertex_count, 
                                    float*& vertices, uint64_t& index_count, 
                                    int32_t*& indices, const Ogre::Vector3& position, 
                                    const Ogre::Quaternion& orient, const Ogre::Vector3& scale) {
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
 
    vertex_count = index_count = 0;
 
    // vertex count is the number of the vertices.
    // index_count is the number of the index values i.d. (index a, index b, index c) * triangles. TODO: correct  here
    
    // Calculate how many vertices and indices we're going to need
    for(unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if(!added_shared)
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        } else {
            vertex_count += submesh->vertexData->vertexCount;
        }
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
    // Allocate space for the vertices and indices
    vertices = new float[vertex_count*3];
    indices = new int32_t[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for(unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = 
                    submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared)) {
            if(submesh->useSharedVertices) {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* pos_elem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(pos_elem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
                pos_elem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                pt = (orient * (pt * scale)) + position;
                vertices[j*3] = pt.x;
                vertices[j*3+1] = pt.y;
                vertices[j*3+2] = pt.z;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t num_tris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32_bit_indices = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if(use32_bit_indices) {
            for(size_t k = 0; k < num_tris*3; ++k) {
                indices[index_offset++] = static_cast<int32_t>(pLong[k]) + static_cast<int32_t>(offset);
            }
        } else {
            for(size_t k = 0; k < num_tris*3; ++k) {
                indices[index_offset++] = static_cast<int32_t>(pShort[k]) +
                                          static_cast<int32_t>(offset);
            }
        }
        
        ibuf->unlock();
        current_offset = next_offset;
    }  
}

// DEBUG DRAW

Ogre::String const DebugDraw::mMaterialName("Void");

DebugDraw::DebugDraw(Ogre::SceneManager* scene_mgr)
{
// create ManualObject
mManualObject = scene_mgr->createManualObject("navigator-debug");

dt::ResourceManager::Get()->AddResourceLocation("","FileSystem");
 
// specify the material (by name) and rendering type
mManualObject->setDynamic(true);
 
// add ManualObject to the RootSceneNode (so it will be visible)
scene_mgr->getRootSceneNode()->attachObject(mManualObject);
}


void DebugDraw::depthMask(bool state) {}

void DebugDraw::texture(bool state) {}

void DebugDraw::begin(duDebugDrawPrimitives prim, float size)
{
    switch (prim)
    {
        case DU_DRAW_POINTS:
                mManualObject->begin(mMaterialName, Ogre::RenderOperation::OT_POINT_LIST);
                break;
        case DU_DRAW_LINES:
                mManualObject->begin(mMaterialName, Ogre::RenderOperation::OT_LINE_LIST);
                break;
        case DU_DRAW_TRIS:
                mManualObject->begin(mMaterialName, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                break;
        case DU_DRAW_QUADS:
                mManualObject->begin(mMaterialName, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
                break;
    };
}

void DebugDraw::vertex(const float* pos, unsigned int color) {
    mManualObject->position(pos[0], pos[1], pos[2]);
    _SetColour(color);
}

void DebugDraw::vertex(const float x, const float y, const float z, unsigned int color) {
    mManualObject->position(x, y, z);
    _SetColour(color);
}

void DebugDraw::vertex(const float* pos, unsigned int color, const float* uv) {
    mManualObject->position(pos[0], pos[1], pos[2]);
    _SetColour(color);
}

void DebugDraw::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v) {
    mManualObject->position(x, y, z);
    _SetColour(color);
}

void DebugDraw::end() {
    mManualObject->end();
}

void DebugDraw::SetVisible(bool visible) {
    mManualObject->setVisible(visible);
}

void DebugDraw::_SetColour(unsigned int color) {
    Ogre::ColourValue color_value;
    color_value.setAsRGBA(color);
    mManualObject->colour(color_value);
}

}
