
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Being.hpp"

#include <Core/ResourceManager.hpp>
#include <Utils/Logger.hpp>
#include <Graphics/MeshComponent.hpp>

#include <OgreMesh.h>

#include <SFML/System/Sleep.hpp>

#include <memory>

dt::Scene* Being::mScene = nullptr;
bool Being::mResourceInitialized = false; 
// GameMap* Being::mGameMap = GameMap::GetGameMapInstance();
std::map<BeingID, BeingPointer> Being::mBeingMap; 
uint32_t Being::mCreatedCount = 0;
double Being::mNowTime = 0;

Being::Being(const Ogre::Vector3& position, uint32_t hp, PlayerID player_id) :
mHP(hp),
mNode(nullptr),
mMeshComponent(nullptr),
mStaringAtBeing(false),
mLookingAt(false),
mPlayerID(player_id),
mMapCell(nullptr) {       
    if(mScene) {
        mBeingID = ++mCreatedCount;
        mNode = mScene->AddChildNode(new dt::Node(dt::Utils::ToString(mBeingID)));
        mNode->SetPosition(position);
        mMapCell = GameMap::GetBeingListIn(position.x + 2000, position.y - 2000); //TODO: this is for getting a different cell, so the Cell will be update with a valid pointer, change this. 
    } else {
        dt::Logger::Get().Error("Being::mScene must point to a valid scene, \
                                 please use Being::SetScene().");
        throw std::runtime_error("invalid scene");
    }
}

Being::~Being() {
    mScene->RemoveChildNode(dt::Utils::ToString(mBeingID));
// dt::Logger::Get().Debug("Being: " + dt::Utils::ToString(mBeingID) + "destructor called");
}

BeingID Being::GetBeingID() {
    return mBeingID;
}

dt::Node* Being::GetNode() {
    return mNode;
}

Ogre::AxisAlignedBox Being::GetBoundingBox() {
    if(mMeshComponent) {
        return mMeshComponent->GetOgreEntity()->getBoundingBox();
    }
    dt::Logger::Get().Debug("Being: " + dt::Utils::ToString(mBeingID) + "has no mesh");
    throw std::runtime_error("nonexistent mesh");
    return Ogre::AxisAlignedBox::BOX_NULL;
}

void Being::MoveTo(const Ogre::Vector3& position) {
    mNode->SetPosition(position);
}

void Being::LookAt(const Ogre::Vector3& position) {
    mLookAtPosition = position;
    mLookingAt = true;
    mStaringAtBeing = false;
}

void Being::StareAtBeing(const BeingPointer& being) {
    QObject::connect(being.get(), SIGNAL(killed(BeingID)), this, SLOT(StopStareAtBeing()));
    mStareAtBeing = being;
    mLookingAt = false;
    mStaringAtBeing = true;
}

void Being::StopStaring() {
    mStaringAtBeing = false;
}

void Being::SetScene(dt::Scene* scene) {
    mScene = scene;
}

void Being::SetMesh(const QString& mesh_name) {
    if(mMeshComponent) {
        mMeshComponent->Deinitialize();
    }
    mMeshComponent = new dt::MeshComponent(mesh_name);
    mNode->AddComponent(mMeshComponent);
}

Ogre::Vector2 Being::Get2DPosition() {
    Ogre::Vector2 position_2D;
    Ogre::Vector3 position_3D = mNode->GetPosition();
    position_2D.x = position_3D.x;
    position_2D.y = position_3D.z;
    return position_2D;
}


BeingPointer Being::FindBeing(const BeingID being_ID) {
    std::map<BeingID, BeingPointer>::iterator it;
    it = mBeingMap.find(being_ID);
    if (it == mBeingMap.end()) {
        dt::Logger::Get().Error("The being with ID: " + dt::Utils::ToString(being_ID) +
                            " couldn't be found");
        throw std::runtime_error("nonexistent being");
    }
//     while(true) {
//     std::map<BeingID, Being*>::iterator it = mBeingMap.find(being_ID);
//         if(it == mBeingMap.end()) {
//             break;
//         }
//         Being* being = it->second;
// //         if(being->IsUsed()) {     
// //             sf::Sleep(50);
// //             continue;
// //         }
//         return being; 
//     }
//     dt::Logger::Get().Error("The being with ID: " + dt::Utils::ToString(being_ID) +
//                                 " couldn't be found");
// //     throw std::runtime_error("nonexistent being");
    return it->second;
}

std::map<BeingID, BeingPointer>& Being::GetAllBeings() {
    return mBeingMap;
}

void Being::_KillBeing() {
//     std::vector<BeingPointer>::iterator it = mBeingMap.at(being_ID);
    mBeingMap.erase(mBeingID);
    mMapCell->erase(mBeingID);
    emit killed(mBeingID);
    return;
}

bool Being::ReduceHP(uint32_t hp) {
    this->mHP -= hp;
    if(this->mHP <= 0) {
        _KillBeing();
        return true;
    }
    return false;
}

void Being::Destroy() {
    _KillBeing();
}

void Being::UpdateBeings(double time_diff) {
    mNowTime += time_diff;
    for(std::map<BeingID, BeingPointer>::iterator it = mBeingMap.begin(); it != mBeingMap.end(); ++it) {
        it->second->Update(time_diff);
    }
}

PlayerID Being::GetPlayerID() {
    return mPlayerID;
} 

MapCell* Being::GetMapCell() {
    return mMapCell;
}

void Being::StopStareAtBeing() {
    mStareAtBeing.reset();
    mStaringAtBeing = false;
}

void Being::OnUpdateStart(double time_diff) {
}


void Being::OnUpdate(double time_diff) {
    if(mLookingAt) {
        mNode->LookAt(mLookAtPosition);
        mLookingAt = false;
    }
    if(mStaringAtBeing) {
        mNode->LookAt(mStareAtBeing->GetNode()->GetPosition());
        mStaringAtBeing = false;
    }
}

void Being::OnUpdateEnd(double time_diff) {
}


void Being::UpdateCell() {
    MapCell* map_cell = GameMap::GetBeingListIn(Get2DPosition());
// dt::Logger::Get().Debug("   " + QString::number((long long int)mMapCell, 16) + " vd  " + QString::number((long long int)map_cell, 16));    
    if(map_cell != mMapCell) {
        mMapCell->erase(mBeingID);
        mMapCell = map_cell;
        mMapCell->insert(std::make_pair(mBeingID, shared_from_this()));
// dt::Logger::Get().Debug(" added n." + QString::number(mBeingID) + " to " + QString::number((long long int)mMapCell, 16));
    }
}

float Being::GetNowTime() {
    return mNowTime;
}


void Being::Update(double time_diff) {
    OnUpdateStart(time_diff);
    OnUpdate(time_diff);
    OnUpdateEnd(time_diff);
}

void Being::InitializeResources() {
    //TODO: review class logic behind this.
    if(!mResourceInitialized) {
        dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }
}

// std::deque<BeingID> Being::CreateBeings(uint32_t number) {
//     std::deque<BeingID> beings;
//     for(uint32_t i = 0; i<number; ++i) {
//         beings.push_front((new Being(Ogre::Vector3::ZERO))->GetBeingID());
//     }
//     return beings;
// }
/*
BeingID Being::GetAvailableID() {
    return mCreatedCount++;
}*/

