
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

#include <QTextStream>

#include "Recast.h"
#include "RecastDebugDraw.h"
#include "RecastDump.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"

static const int MAX_CONVEXVOL_PTS = 12;

struct ConvexVolume
{
        float verts[MAX_CONVEXVOL_PTS*3];
        float hmin, hmax;
        int nverts;
        int area;
};

namespace dt {

NavigationManager::NavigationManager() : 
mVerticesNumber(0),
mIndicesNumber(0),
mVertices(nullptr),
mIndices(nullptr),
mDebugDraw(nullptr) {}

NavigationManager::~NavigationManager()
{
    log.flush();
    log.close();
}


void NavigationManager::Initialize() {
    mFilter.setIncludeFlags(SAMPLE_POLYFLAGS_ALL ^ SAMPLE_POLYFLAGS_DISABLED);
    mFilter.setExcludeFlags(0);
    
    mFilter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_WATER, 10.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_ROAD, 1.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_GRASS, 2.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_JUMP, 1.5f);
    
    mNavQuery = dtAllocNavMeshQuery();
    
    mCFG = CreateDefaultConfig();
    
    log.open("navigation-manager-log.txt", std::ios::out);
    if(!log.good()) {
        dt::Logger::Get().Debug("unable to write a log for navigation manager");
    }
    log << "# Navigation Manager Log.\n";
}

void NavigationManager::ResetCommonSetting() {
   mCFG = CreateDefaultConfig();
}

rcConfig NavigationManager::CreateDefaultConfig()
{
    rcConfig config;
    memset(&config, 0, sizeof(config));
    config.cs = 0.3f;
    config.ch = 0.2f;
    config.walkableSlopeAngle = 45.0f;
    config.walkableHeight = (int)ceilf(2.0f / config.ch);
    config.walkableClimb = (int)floorf(0.9f / config.ch);
    config.walkableRadius =  (int)ceilf(0.6f / config.cs);
    config.maxEdgeLen = (int)(12.0f / 0.3f); 12;
    config.maxSimplificationError = 1.3f;
    config.minRegionArea = (int)rcSqr(8);   
    config.mergeRegionArea = (int)rcSqr(20);
    config.maxVertsPerPoly = (int)6.0f;
    config.detailSampleDist = 6.0f < 0.9f ? 0 : 0.3f *  6.0f;
    config.detailSampleMaxError = 0.2f * 1.0f;
    return config;
}

void NavigationManager::SetConfig(const rcConfig& config) {
    mCFG = config;
    
    // Some data must be modified, the original provided by the user
    // is stored in this class private data.
    mAgentHeight = mCFG.walkableHeight;
    mCFG.walkableHeight = (int)ceilf(mAgentHeight / config.ch);
    mAgentMaxClimb = mCFG.walkableClimb;
    mCFG.walkableClimb = (int)floorf(mAgentMaxClimb / config.ch);
    mAgentRadius = mCFG.walkableRadius;
    mCFG.walkableRadius = (int)ceilf(mAgentRadius / config.cs);
}

rcConfig NavigationManager::GetConfig() {
    return mCFG;
}

void NavigationManager::CreateDebugDrawer(Ogre::SceneManager* scene_mgr) {
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

Crowd* NavigationManager::CreateCrowd() {
    Crowd* crowd = new Crowd(mNavMesh);
    mCrowdList.push_back(crowd);
    return crowd;
}

void NavigationManager::UpdateFrame(double simulation_frame_time) {
    for(std::deque<Crowd*>::iterator it = mCrowdList.begin(); it < mCrowdList.end(); ++it) {
        (*it)->UpdateFrame(simulation_frame_time);
    }
}



bool NavigationManager::_RastetizeMesh() {
    
    float bmin[3];
    float bmax[3];
    log << " Verts number " << mVerticesNumber << std::endl;
    
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
    
    dt::Logger::Get().Debug( "Building navigation:");
    dt::Logger::Get().Debug( dt::Utils::ToString(mCFG.width) +  " x " + dt::Utils::ToString(mCFG.height) + "cells");
    dt::Logger::Get().Debug( dt::Utils::ToString(mVerticesNumber/1000.0f) + "K verts, " + dt::Utils::ToString(ntris/1000.0f) + "K tris");
    
    //
    // Step 2. Rasterize input polygon soup.
    //
        
    // Allocate voxel heightfield where we rasterize our input data to.
    mSolid = rcAllocHeightfield();
    if(!mSolid)
    {
            dt::Logger::Get().Debug( "Cannot alloc an height field");
            return false;
    }
    if(!rcCreateHeightfield(&mCTX, *mSolid, mCFG.width, mCFG.height, mCFG.bmin, mCFG.bmax, mCFG.cs, mCFG.ch))
    {
            dt::Logger::Get().Debug( "Cannot create an height field ");
            return false;
    }
    
    // Allocate array that can hold triangle area types.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.

    mTriareas = new unsigned char[ntris];
    if(!mTriareas)
    {
            dt::Logger::Get().Debug( "Cannot alloc array for triangle areas");
            return false;
    }
    
    for(int i = 0;i<(int)mIndicesNumber; ++i) {
        log << "vertex of triangle n. " << i/3 << " x= " << mVertices[mIndices[i]] << 
                                                " y= " << mVertices[mIndices[i]+1] << 
                                                " z= " << mVertices[mIndices[i]+2] << std::endl;
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

bool NavigationManager::_FilterWalkableSurface()
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
    if(!mCHF)
    {
        dt::Logger::Get().Debug( "Cannot alloc a compact heightfield");
        return false;
    }
    if(!rcBuildCompactHeightfield(&mCTX, mCFG.walkableHeight, mCFG.walkableClimb, *mSolid, *mCHF))
    {
        dt::Logger::Get().Debug( "Cannot build a compact heightfield");
        return false;
    }
    
//         if(!m_keepInterResults)
//         {
//                 rcFreeHeightField(m_solid);
//                 m_solid = 0;
//         }
            
    // Erode the walkable area by agent radius.
    if(!rcErodeWalkableArea(&mCTX, mCFG.walkableRadius, *mCHF)) //TODO copy this function from demo
    {
        dt::Logger::Get().Debug( "ErodeWalkableArea() failed");
        return false;
    }

    
    // (Optional) Mark areas. TODO: commented here
//         const ConvexVolume* vols = m_geom->getConvexVolumes();
//         for(int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
//                 rcMarkConvexPolyArea(&m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);
    
    bool m_monotonePartitioning = true;
    
    if(m_monotonePartitioning)
    {
            // Partition the walkable surface into simple regions without holes.
            // Monotone partitioning does not need distancefield.
            if(!rcBuildRegionsMonotone(&mCTX, *mCHF, 0, mCFG.minRegionArea, mCFG.mergeRegionArea))
            {
                    dt::Logger::Get().Debug("Could not build regions.");
                    return false;
            }
    }
    else
    {
            // Prepare for region partitioning, by calculating distance field along the walkable surface.
            if(!rcBuildDistanceField(&mCTX, *mCHF))
            {
                    dt::Logger::Get().Debug("Could not build distance field.");
                    return false;
            }

            // Partition the walkable surface into simple regions without holes.
            if(!rcBuildRegions(&mCTX, *mCHF, 0, mCFG.minRegionArea, mCFG.mergeRegionArea))
            {
                    dt::Logger::Get().Debug("Could not build regions.");
                    return false;
            }
    }
}

bool NavigationManager::_TraceAndSimplifyRegion()
{
    // Create contours.
    mCSet = rcAllocContourSet();
    if(!mCSet)
    {
            dt::Logger::Get().Debug("Cannot alloc a contour set.");
            return false;
    }
    if(!rcBuildContours(&mCTX, *mCHF, mCFG.maxSimplificationError, mCFG.maxEdgeLen, *mCSet))
    {
        dt::Logger::Get().Debug("Could not build the contour set.");
            return false;
    }
}

bool NavigationManager::_BuildPolygonsMesh() {
    // Build polygon navmesh from the contours.
    mPMesh = rcAllocPolyMesh();
    if(!mPMesh)
    {
            dt::Logger::Get().Debug("Cannot alloc a poly mesh");
            return false;
    }
    if(!rcBuildPolyMesh(&mCTX, *mCSet, mCFG.maxVertsPerPoly, *mPMesh))
    {
            dt::Logger::Get().Debug("Could not build the poly mesh");
            return false;
    }
}


bool NavigationManager::_BuildDetailMesh() {
    mDMesh = rcAllocPolyMeshDetail();
    if(!mDMesh)
    {
            mCTX.log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
            return false;
    }

    if(!rcBuildPolyMeshDetail(&mCTX, *mPMesh, *mCHF, mCFG.detailSampleDist, mCFG.detailSampleMaxError, *mDMesh))
    {
            mCTX.log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
            return false;
    }

//     if(!mKeepTempResult)
//     {
//             rcFreeCompactHeightfield(mCHF);
//             mCHF = 0;
//             rcFreeContourSet(mCSet);
//             mCSet = 0;
//     }

    // At this point the navigation mesh data is ready, you can access it from m_pmesh.
    // See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
}

bool NavigationManager::_CreateDetourData() {
    // The GUI may allow more max points per polygon than Detour can handle.
    // Only build the detour navmesh if we do not exceed the limit.
    if(mCFG.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
    {
            unsigned char* navData = 0;
            int navDataSize = 0;

            // Update poly flags from areas.
            for(int i = 0; i < mPMesh->npolys; ++i)
            {
                    if(mPMesh->areas[i] == RC_WALKABLE_AREA)
                            mPMesh->areas[i] = SAMPLE_POLYAREA_GROUND;
                            
                    if(mPMesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
                            mPMesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
                            mPMesh->areas[i] == SAMPLE_POLYAREA_ROAD)
                    {
                            mPMesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
                    }
                    else if(mPMesh->areas[i] == SAMPLE_POLYAREA_WATER)
                    {
                            mPMesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
                    }
                    else if(mPMesh->areas[i] == SAMPLE_POLYAREA_DOOR)
                    {
                            mPMesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
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
//                 params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
//                 params.offMeshConRad = m_geom->getOffMeshConnectionRads();
//                 params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
//                 params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
//                 params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
//                 params.offMeshConUserID = m_geom->getOffMeshConnectionId();
            params.offMeshConCount = 0;
            params.walkableHeight = mAgentHeight;
            params.walkableRadius = mAgentRadius;
            params.walkableClimb = mAgentMaxClimb;
            rcVcopy(params.bmin, mPMesh->bmin);
            rcVcopy(params.bmax, mPMesh->bmax);
            params.cs = mCFG.cs;
            params.ch = mCFG.ch;
            params.buildBvTree = true;
            
            if(!dtCreateNavMeshData(&params, &navData, &navDataSize))
            {
                dt::Logger::Get().Debug("Could not create Detour navmesh data.");
                return false;
            }
            
            mNavMesh = dtAllocNavMesh();
            if(!mNavMesh)
            {
                dtFree(navData);
                dt::Logger::Get().Debug("Could not create Detour navmesh");
                return false;
            }
            
            dtStatus status;
            
            status = mNavMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
            if(dtStatusFailed(status))
            {
                    dtFree(navData);
                dt::Logger::Get().Debug("Could not init Detour navmesh");
                return false;
            }
            
            status = mNavQuery->init(mNavMesh, 2048);
            if(dtStatusFailed(status))
            {
                dt::Logger::Get().Debug("Could not init Detour navmesh query");
                return false;
            }
    }
    
    mCTX.stopTimer(RC_TIMER_TOTAL);

    // Show performance stats.
    duLogBuildTimes(mCTX, mCTX.getAccumulatedTime(RC_TIMER_TOTAL));
    dt::Logger::Get().Debug("Polymesh: " + dt::Utils::ToString(mVerticesNumber) + " vertices and " + dt::Utils::ToString(mIndicesNumber/3) + " triangles");
    
    mTotalBuildTime = mCTX.getAccumulatedTime(RC_TIMER_TOTAL)/1000.0f;
    
    duDebugDrawNavMeshPolysWithFlags(mDebugDraw, *mNavMesh, SAMPLE_POLYFLAGS_ALL, duRGBA(0,0,0,128));
}




bool NavigationManager::BuildMap() {
    
    // Debug code, print all the vertices.
    for(uint64_t i = 0; i < mVerticesNumber; i++) {
        log << "vertex n." << i <<      " x=" <<  mVertices[i*3] <<
                                        " y=" <<  mVertices[i*3+1] << 
                                        " x=" <<  mVertices[i*3+2] << std::endl;
    }
    log.flush();
    
    _RastetizeMesh();
    
    _FilterWalkableSurface();

    _PartitionSurfaceToRegions();
   
    _TraceAndSimplifyRegion();
    
    _BuildPolygonsMesh();
    
    _BuildDetailMesh();
    
    _CreateDetourData();
    
    return true;
}


void NavigationManager::Deinitialize() {}

NavigationManager* NavigationManager::Get() {
    return Root::GetInstance().GetNavigationManager();
}

// bool NavigationManager::AddNavigation(QString script, QString name) {
//     return true;
// }

void NavigationManager::AddMesh(Ogre::Mesh& mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale)
{
    uint64_t new_vertex_count(0), new_index_count(0);
    float* new_vertices = nullptr;
    int32_t* new_indices = nullptr;
    
    _GetMeshVertices(&mesh, new_vertex_count, new_vertices, new_index_count, new_indices, 
                     position, orient, scale);
    
        // Debug code, print all the vertices.
    static uint8_t mesh_number = 0;
    log << " Adding mesh number " << ++mesh_number << std::endl;
    
    for(uint64_t i = 0; i < new_vertex_count; i++) {
        log << " new vertex n." << i <<      " x=" <<  new_vertices[i*3] <<
                                        " y=" <<  new_vertices[i*3+1] << 
                                        " x=" <<  new_vertices[i*3+2] << std::endl;
    }
    log.flush();
    
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
    
    for(uint64_t i = 0; i < mIndicesNumber; i += 3) {
        log << "triangle n." << i/3 <<  " a=" << mIndices[i] <<
                                        " b=" << mIndices[i+1] << 
                                        " c=" << mIndices[i+2] << std::endl;
    }   
}

bool NavigationManager::UpdateNavQuery()
{
    mSearchDistance[0] = 2;
    mSearchDistance[1] = 4;
    mSearchDistance[2] = 2;
    
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
    if(mNPolys)
    {
            // In case of partial path, make sure the end point is clamped to the last polygon.
            float epos[3];
            dtVcopy(epos, end_pos);
            if(mPolys[mNPolys-1] != mEndRef)
                    mNavQuery->closestPointOnPoly(mPolys[mNPolys-1], end_pos, epos);
            
            mNavQuery->findStraightPath(start_pos, epos, mPolys, mNPolys,
                                                                    mStraightPath, mStraightPathFlags,
                                                                        mStraightPathPolys, &mNStraightPath, MAX_POLYS);
    }
            
}

std::deque<Ogre::Vector3> NavigationManager::FindPath(const Ogre::Vector3& begin, const Ogre::Vector3& end) {
    
    mBeginPosition = begin;
    mEndPosition = end;
    
    UpdateNavQuery();
    
//     Ogre::Vector3 point;
    std::deque<Ogre::Vector3> path;
    // Float triplets to Ogre vector3.
    for(uint32_t i = 0; i < 9000 && i < mNStraightPath; ++i) {
//         point.x = mStraightPath[i*3];
//         point.y = mStraightPath[i*3+1];
//         point.z = mStraightPath[i*3+2];
        path.push_back(dt::Utils::FloatsToOgreVector3(&mStraightPath[i*3]));
    }
    
    return path;
}

void NavigationManager::AddPathToComponent(const Ogre::Vector3& begin, const Ogre::Vector3& end, FollowPathComponent& path) {
    mBeginPosition = begin;
    mEndPosition = end;
    
    UpdateNavQuery();
    
    // Float triplets to Ogre vector3.
    for(uint32_t i = 0; i < 9000 && i < mNStraightPath; ++i) {
        path.AddPoint(dt::Utils::FloatsToOgreVector3(&mStraightPath[i*3]));
    }
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
    for( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
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
    for(unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
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
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if( use32bitindexes )
        {
            for( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<int32_t>(pLong[k]) + static_cast<int32_t>(offset);
            }
        }
        else
        {
            for( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<int32_t>(pShort[k]) +
                                          static_cast<int32_t>(offset);
            }
        }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
    
}

// DEBUG DRAW

DebugDraw::DebugDraw(Ogre::SceneManager* scene_mgr)
{
    // create ManualObject
mManualObject = scene_mgr->createManualObject("navigator-debug");
 
// specify the material (by name) and rendering type
mManualObject->setDynamic(true);
mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_POINT_LIST);
 
// debug points
mManualObject->position(0, 100, 0);
mManualObject->position(3, 100, 3);
 
// tell Ogre, your definition has finished
mManualObject->end();
 
// add ManualObject to the RootSceneNode (so it will be visible)
scene_mgr->getRootSceneNode()->attachObject(mManualObject);
}


void DebugDraw::depthMask(bool state)
{}

void DebugDraw::texture(bool state)
{}

void DebugDraw::begin(duDebugDrawPrimitives prim, float size)
{
        switch (prim)
        {
                case DU_DRAW_POINTS:
                        mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_POINT_LIST);
                        break;
                case DU_DRAW_LINES:
                        mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
                        break;
                case DU_DRAW_TRIS:
                        mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
                        break;
                case DU_DRAW_QUADS:
                        mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP); // TODO will draw something wrong
                        break;
        };
}

void DebugDraw::vertex(const float* pos, unsigned int color) {
    //TODO IMPLEMENT
    mManualObject->position(pos[0], pos[1], pos[2]);
    Ogre::ColourValue color_value;
    color_value.setAsRGBA(color);
    mManualObject->colour(color_value);
}

void DebugDraw::vertex(const float x, const float y, const float z, unsigned int color) {
    //TODO IMPLEMENT
    mManualObject->position(x, y, z);
    Ogre::ColourValue color_value;
    color_value.setAsRGBA(color);
    mManualObject->colour(color_value);
}

void DebugDraw::vertex(const float* pos, unsigned int color, const float* uv) {
    //TODO IMPLEMENT
    mManualObject->position(pos[0], pos[1], pos[2]);
    Ogre::ColourValue color_value;
    color_value.setAsRGBA(color);
    mManualObject->colour(color_value);
}

void DebugDraw::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v) {
    mManualObject->position(x, y, z);
    Ogre::ColourValue color_value;
    color_value.setAsRGBA(color);
    mManualObject->colour(color_value);
}

void DebugDraw::end() {
    mManualObject->end();
}

void DebugDraw::SetVisible(bool visible) {
    mManualObject->setVisible(visible);
}

Crowd::Crowd(dtNavMesh* nav_mesh) :
//     m_sample(0),
    m_oldFlags(0),
    m_targetRef(0),
    m_expandSelectedDebugDraw(true),
    m_showCorners(false),
    m_showCollisionSegments(false),
    m_showPath(false),
    m_showVO(false),
    m_showOpt(false),
    m_showNeis(false),
    m_expandDebugDraw(false),
    m_showLabels(false),
    m_showGrid(false),
    m_showNodes(false),
    m_showPerfGraph(false),
    m_expandOptions(true),
    m_anticipateTurns(true),
    m_optimizeVis(true),
    m_optimizeTopo(true),
    m_obstacleAvoidance(true),
    m_obstacleAvoidanceType(3.0f),
    m_separation(false),
    m_separationWeight(2.0f),
    m_run(true),
    m_mode(TOOLMODE_CREATE),
    mNavMesh(nav_mesh){
    memset(m_trails, 0, sizeof(m_trails));
    
    m_vod = dtAllocObstacleAvoidanceDebugData();
    m_vod->init(2048);
    
    memset(&m_agentDebug, 0, sizeof(m_agentDebug));
    m_agentDebug.idx = -1;
    m_agentDebug.vod = m_vod;
    
//         if (m_sample != sample)
//         {
//                 m_sample = sample;
//                 m_oldFlags = m_sample->getNavMeshDrawFlags();
//                 m_sample->setNavMeshDrawFlags(m_oldFlags & ~DU_DRAWNAVMESH_CLOSEDLIST);
//         }
                
//         dtNavMesh* nav = m_sample->getNavMesh();
//         dtCrowd* mCrowd = m_sample->getCrowd();
        if (mNavMesh)
        {
                mCrowd.init(MAX_AGENTS, MAX_AGENT_RADIUS, mNavMesh);

                // Make polygons with 'disabled' flag invalid.
                mCrowd.getEditableFilter()->setExcludeFlags(SAMPLE_POLYFLAGS_DISABLED);
                
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
//     if (m_sample)
//     {
//             m_sample->setNavMeshDrawFlags(m_oldFlags);
//     }
    
    dtFreeObstacleAvoidanceDebugData(m_vod);
}

void Crowd::Initialize() {

}

void Crowd::Deinitialize() {

}

// void Crowd::handleUpdate(const float dt) {
// 
// }
// 
// void Crowd::updateTick(const float dt) {
// 
// }



AgentComponent* Crowd::CreateAgentComponent(const Ogre::Vector3& position, const dtCrowdAgentParams& ap, const QString& name) {
    float pos[3];
    dt::Utils::OgreVector3ToFloats(position, pos);
    
    int index = mCrowd.addAgent(pos, &ap);
    if (index != -1)
    {
            if (m_targetRef)
                    mCrowd.requestMoveTarget(index, m_targetRef, m_targetPos);

//             // Init trail
//             AgentTrail* trail = &m_trails[index];
//             for (int i = 0; i < AGENT_MAX_TRAIL; ++i)
//                     dtVcopy(&trail->trail[i*3], p);
//             trail->htrail = 0;
    }

    AgentComponent* agent_component = new AgentComponent(this, index, name);
    return agent_component;
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

void Crowd::updateAgentParams() {
//         dtCrowd* mCrowd = m_sample->getCrowd();
//         if (!mCrowd) return;

        unsigned char updateFlags = 0;
        unsigned char obstacleAvoidanceType = 0;
        
        if (m_anticipateTurns)
                updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
        if (m_optimizeVis)
                updateFlags |= DT_CROWD_OPTIMIZE_VIS;
        if (m_optimizeTopo)
                updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
        if (m_obstacleAvoidance)
                updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
        if (m_obstacleAvoidance)
                updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
        if (m_separation)
                updateFlags |= DT_CROWD_SEPARATION;
        
        obstacleAvoidanceType = (unsigned char)m_obstacleAvoidanceType;

        dtCrowdAgentParams params;
        
        for (int i = 0; i < mCrowd.getAgentCount(); ++i)
        {
                const dtCrowdAgent* ag = mCrowd.getAgent(i);
                if (!ag->active) continue;
                memcpy(&params, &ag->params, sizeof(dtCrowdAgentParams));
                params.updateFlags = updateFlags;
                params.obstacleAvoidanceType = obstacleAvoidanceType;
                params.separationWeight = m_separationWeight;
                mCrowd.updateAgentParameters(i, &params);
        }       
}

void Crowd::UpdateFrame(double simulation_frame_time) {
        if(!mNavMesh) { 
            return;
        }

//         TimeVal startTime = getPerfTime();
        
        mCrowd.update(simulation_frame_time, &m_agentDebug); //TODO
        
        
//         TimeVal endTime = getPerfTime();
        
        // Update agent trails
//         for (int i = 0; i < mCrowd.getAgentCount(); ++i)
//         {
//                 const dtCrowdAgent* ag = mCrowd.getAgent(i);
//                 AgentTrail* trail = &m_trails[i];
//                 if (!ag->active)
//                         continue;
//                 // Update agent movement trail.
//                 trail->htrail = (trail->htrail + 1) % AGENT_MAX_TRAIL;
//                 dtVcopy(&trail->trail[trail->htrail*3], ag->npos);
//         }
        
//         m_agentDebug.vod->normalizeSamples();
//         
//         m_mCrowdSampleCount.addSample((float)mCrowd.getVelocitySampleCount());
//         m_mCrowdTotalTime.addSample(getPerfDeltaTimeUsec(startTime, endTime) / 1000.0f);
}


}