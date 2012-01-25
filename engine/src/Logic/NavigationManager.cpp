
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

NavigationManager::NavigationManager(Ogre::SceneManager* scene_mgr) : 
mVerticesNumber(0),
mIndicesNumber(0),
mVertices(nullptr),
mIndices(nullptr)
{
    mFilter.setIncludeFlags(SAMPLE_POLYFLAGS_ALL ^ SAMPLE_POLYFLAGS_DISABLED);
    mFilter.setExcludeFlags(0);
    
    mFilter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_WATER, 10.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_ROAD, 1.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_GRASS, 2.0f);
    mFilter.setAreaCost(SAMPLE_POLYAREA_JUMP, 1.5f);
    
    mNavQuery = dtAllocNavMeshQuery();
    
    mDebugDraw = new DebugDraw(scene_mgr);
    
//     log.clear();
    log.open("navigation-manager-log.txt", std::ios::out);
    if(!log.good()) {
        dt::Logger::Get().Debug("unable to write a log for navigation manager");
    }
    log << "# Navigation Manager Log.\n";
}

NavigationManager::~NavigationManager()
{
    log.flush();
    log.close();
}


void NavigationManager::Initialize() {}

void NavigationManager::ResetCommonSetting() {
    // Init build configuration from GUI
    memset(&mCFG, 0, sizeof(mCFG));
    mCFG.cs = 0.3f;
    mCFG.ch = 0.2f;
    mCFG.walkableSlopeAngle = 45.0f;
    mCFG.walkableHeight = (int)ceilf(2.0f / mCFG.ch);
    mCFG.walkableClimb = (int)floorf(0.9f / mCFG.ch);
    mCFG.walkableRadius =  (int)ceilf(0.6f / mCFG.cs);
    mCFG.maxEdgeLen = (int)(12.0f / 0.3f); 12;
    mCFG.maxSimplificationError = 1.3f;
    mCFG.minRegionArea = (int)rcSqr(8);   
    mCFG.mergeRegionArea = (int)rcSqr(20);
    mCFG.maxVertsPerPoly = (int)6.0f;
    mCFG.detailSampleDist = 6.0f < 0.9f ? 0 : 0.3f *  6.0f;
    mCFG.detailSampleMaxError = 0.2f * 1.0f;
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
    if (!mSolid)
    {
            dt::Logger::Get().Debug( "Cannot alloc an height field");
            return false;
    }
    if (!rcCreateHeightfield(&mCTX, *mSolid, mCFG.width, mCFG.height, mCFG.bmin, mCFG.bmax, mCFG.cs, mCFG.ch))
    {
            dt::Logger::Get().Debug( "Cannot create an height field ");
            return false;
    }
    
    // Allocate array that can hold triangle area types.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.

    mTriareas = new unsigned char[ntris];
    if (!mTriareas)
    {
            dt::Logger::Get().Debug( "Cannot alloc array for triangle areas");
            return false;
    }
    
//TODO: delete this debug code
for (int i = 0;i<(int)mIndicesNumber; ++i) {
     log << "vertex of triangle n. " << i/3 << " x= " << mVertices[mIndices[i]] << 
                                               " y= " << mVertices[mIndices[i]+1] << 
                                               " z= " << mVertices[mIndices[i]+2] << std::endl;
                                               
    return true;
    
}

    int32_t numTris = mIndicesNumber / 3;
    
    // Find triangles which are walkable based on their slope and rasterize them.
    // If your input data is multiple meshes, you can transform them here, calculate
    // the are type for each of the meshes and rasterize them.
    memset(mTriareas, 0, ntris*sizeof(unsigned char));
    rcMarkWalkableTriangles(&mCTX, mCFG.walkableSlopeAngle, mVertices, mVerticesNumber, mIndices, numTris, mTriareas);
    rcRasterizeTriangles(&mCTX, mVertices, mVerticesNumber, mIndices, mTriareas, numTris, *mSolid, mCFG.walkableClimb);

    if (!mKeepTempResult)
        {
                delete [] mTriareas;
                mTriareas = 0;
        }
        
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
    if (!mCHF)
    {
        dt::Logger::Get().Debug( "Cannot alloc a compact heightfield");
        return false;
    }
    if (!rcBuildCompactHeightfield(&mCTX, mCFG.walkableHeight, mCFG.walkableClimb, *mSolid, *mCHF))
    {
        dt::Logger::Get().Debug( "Cannot build a compact heightfield");
        return false;
    }
    
//         if (!m_keepInterResults)
//         {
//                 rcFreeHeightField(m_solid);
//                 m_solid = 0;
//         }
            
    // Erode the walkable area by agent radius.
    if (!rcErodeWalkableArea(&mCTX, mCFG.walkableRadius, *mCHF)) //TODO copy this function from demo
    {
        dt::Logger::Get().Debug( "ErodeWalkableArea() failed");
        return false;
    }

    
    // (Optional) Mark areas. TODO: commented here
//         const ConvexVolume* vols = m_geom->getConvexVolumes();
//         for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
//                 rcMarkConvexPolyArea(&m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);
    
    bool m_monotonePartitioning = true;
    
    if (m_monotonePartitioning)
    {
            // Partition the walkable surface into simple regions without holes.
            // Monotone partitioning does not need distancefield.
            if (!rcBuildRegionsMonotone(&mCTX, *mCHF, 0, mCFG.minRegionArea, mCFG.mergeRegionArea))
            {
                    dt::Logger::Get().Debug("Could not build regions.");
                    return false;
            }
    }
    else
    {
            // Prepare for region partitioning, by calculating distance field along the walkable surface.
            if (!rcBuildDistanceField(&mCTX, *mCHF))
            {
                    dt::Logger::Get().Debug("Could not build distance field.");
                    return false;
            }

            // Partition the walkable surface into simple regions without holes.
            if (!rcBuildRegions(&mCTX, *mCHF, 0, mCFG.minRegionArea, mCFG.mergeRegionArea))
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
    if (!mCSet)
    {
            dt::Logger::Get().Debug("Cannot alloc a contour set.");
            return false;
    }
    if (!rcBuildContours(&mCTX, *mCHF, mCFG.maxSimplificationError, mCFG.maxEdgeLen, *mCSet))
    {
        dt::Logger::Get().Debug("Could not build the contour set.");
            return false;
    }
}

bool NavigationManager::_BuildPolygonsMesh() {
    // Build polygon navmesh from the contours.
    mPMesh = rcAllocPolyMesh();
    if (!mPMesh)
    {
            dt::Logger::Get().Debug("Cannot alloc a poly mesh");
            return false;
    }
    if (!rcBuildPolyMesh(&mCTX, *mCSet, mCFG.maxVertsPerPoly, *mPMesh))
    {
            dt::Logger::Get().Debug("Could not build the poly mesh");
            return false;
    }
}


bool NavigationManager::_BuildDetailMesh() {
    mDMesh = rcAllocPolyMeshDetail();
    if (!mDMesh)
    {
            mCTX.log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
            return false;
    }

    if (!rcBuildPolyMeshDetail(&mCTX, *mPMesh, *mCHF, mCFG.detailSampleDist, mCFG.detailSampleMaxError, *mDMesh))
    {
            mCTX.log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
            return false;
    }

    if (!mKeepTempResult)
    {
            rcFreeCompactHeightfield(mCHF);
            mCHF = 0;
            rcFreeContourSet(mCSet);
            mCSet = 0;
    }

    // At this point the navigation mesh data is ready, you can access it from m_pmesh.
    // See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
}

bool NavigationManager::_CreateDetourData() {
    // The GUI may allow more max points per polygon than Detour can handle.
    // Only build the detour navmesh if we do not exceed the limit.
    if (mCFG.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
    {
            unsigned char* navData = 0;
            int navDataSize = 0;

            // Update poly flags from areas.
            for (int i = 0; i < mPMesh->npolys; ++i)
            {
                    if (mPMesh->areas[i] == RC_WALKABLE_AREA)
                            mPMesh->areas[i] = SAMPLE_POLYAREA_GROUND;
                            
                    if (mPMesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
                            mPMesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
                            mPMesh->areas[i] == SAMPLE_POLYAREA_ROAD)
                    {
                            mPMesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
                    }
                    else if (mPMesh->areas[i] == SAMPLE_POLYAREA_WATER)
                    {
                            mPMesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
                    }
                    else if (mPMesh->areas[i] == SAMPLE_POLYAREA_DOOR)
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
            params.walkableHeight = 2.0f;
            params.walkableRadius = 0.6f;
            params.walkableClimb = 0.9f;
            rcVcopy(params.bmin, mPMesh->bmin);
            rcVcopy(params.bmax, mPMesh->bmax);
            params.cs = mCFG.cs;
            params.ch = mCFG.ch;
            params.buildBvTree = true;
            
            if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
            {
                dt::Logger::Get().Debug("Could not create Detour navmesh data.");
                return false;
            }
            
            mNavMesh = dtAllocNavMesh();
            if (!mNavMesh)
            {
                dtFree(navData);
                dt::Logger::Get().Debug("Could not create Detour navmesh");
                return false;
            }
            
            dtStatus status;
            
            status = mNavMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
            if (dtStatusFailed(status))
            {
                    dtFree(navData);
                dt::Logger::Get().Debug("Could not init Detour navmesh");
                return false;
            }
            
            status = mNavQuery->init(mNavMesh, 2048);
            if (dtStatusFailed(status))
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
#define DRAWMESH 
#ifdef DRAWMESH
//        duDebugDrawNavMeshPolysWithFlags(&dd, *m_navMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
#endif
//         UpdateNavQuery();
    
    duDebugDrawNavMeshPolysWithFlags(mDebugDraw, *mNavMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
    mDebugDraw->begin(DU_DRAW_QUADS,1);
    mDebugDraw->vertex(1,1,1,1);
    mDebugDraw->vertex(1,1,-1,1);
    mDebugDraw->vertex(-1,1,1,1);
    mDebugDraw->vertex(-1,1,-1,1);
    mDebugDraw->end();
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
    return /*mIstance;*/ nullptr; //TODO: 
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
    mVertices = vertices;
    mVerticesNumber += new_vertex_count;
    
    // Copy all indices, new and old in a new array, erase the old array.
    int32_t* indices = new int32_t[static_cast<size_t>(mIndicesNumber) + new_index_count];
    memcpy(indices, mIndices, sizeof(int32_t) * mIndicesNumber);
    memcpy(&indices[mIndicesNumber], new_indices, sizeof(int32_t) * new_index_count);
    delete[] mIndices;
    delete[] new_indices;

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
    _OgreVector3ToFloats(mBeginPosition, start_pos);
    mNavQuery->findNearestPoly(start_pos, mSearchDistance, &mFilter, &mStartRef, 0);
    
    float end_pos[3];
    _OgreVector3ToFloats(mEndPosition, end_pos);
    mNavQuery->findNearestPoly(end_pos, mSearchDistance, &mFilter, &mEndRef, 0);
    
    
    dtStatus status = mNavQuery->findPath(mStartRef, mEndRef, start_pos, end_pos, &mFilter, mPolys, &mNPolys, MAX_POLYS);
    if(status & (DT_INVALID_PARAM | DT_FAILURE)) {
        dt::Logger::Get().Debug("findpath() failed, invalid param or other problem");
    }
    mNStraightPath = 0;
    if (mNPolys)
    {
            // In case of partial path, make sure the end point is clamped to the last polygon.
            float epos[3];
            dtVcopy(epos, end_pos);
            if (mPolys[mNPolys-1] != mEndRef)
                    mNavQuery->closestPointOnPoly(mPolys[mNPolys-1], end_pos, epos);
            
            mNavQuery->findStraightPath(start_pos, epos, mPolys, mNPolys,
                                                                    mStraightPath, mStraightPathFlags,
                                                                        mStraightPathPolys, &mNStraightPath, MAX_POLYS);
    }
            
}

std::deque< Ogre::Vector3 > NavigationManager::FindPath(const Ogre::Vector3& begin, const Ogre::Vector3& end) {
    
    mBeginPosition = begin;
    mEndPosition = end;
    
    UpdateNavQuery();
    
    Ogre::Vector3 point;
    std::deque<Ogre::Vector3> path;
    // Float triplets to Ogre vector3.
    for(uint32_t i = 0; i < 9000 && i < mNStraightPath; ++i) {
        point.x = mStraightPath[i*3];
        point.y = mStraightPath[i*3+1];
        point.z = mStraightPath[i*3+2];
        dt::Logger::Get().Debug( 
                        "PATH: x=" +  dt::Utils::ToString(point.x) + 
                        " y=" +  dt::Utils::ToString(point.y) + 
                        " z=" +  dt::Utils::ToString(point.z));
        path.push_back(_FloatsToOgreVector3(&mStraightPath[i*3]));
    }
    
    return path;
}

Ogre::Vector3 NavigationManager::_FloatsToOgreVector3(float* float_vector) { 
    Ogre::Vector3 ogre_vector;
    ogre_vector.x = float_vector[0];
    ogre_vector.y = float_vector[1];
    ogre_vector.z = float_vector[2];
    return ogre_vector;
}

void NavigationManager::_OgreVector3ToFloats(const Ogre::Vector3& ogre_vector, float* float_vector) {
    float_vector[0] = ogre_vector.x;
    float_vector[1] = ogre_vector.y;
    float_vector[2] = ogre_vector.z;
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
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
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
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
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
 
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<int32_t>(pLong[k]) + static_cast<int32_t>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
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

void DebugDraw::vertex(const float* pos, unsigned int color)
{
    //TODO IMPLEMENT
    mManualObject->position(pos[0], pos[1]+1, pos[2]);
}

void DebugDraw::vertex(const float x, const float y, const float z, unsigned int color)
{
    //TODO IMPLEMENT
    mManualObject->position(x, y+1, z);
}

void DebugDraw::vertex(const float* pos, unsigned int color, const float* uv)
{
    //TODO IMPLEMENT
    mManualObject->position(pos[0], pos[1]+1, pos[2]);
}

void DebugDraw::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
{
    mManualObject->position(x, y+1, z);
}

void DebugDraw::end()
{
    mManualObject->end();
}

}


