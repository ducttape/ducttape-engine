
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Building.hpp"

#include <Core/ResourceManager.hpp>

bool Building::mResourceInitialized = false;

Building::Building(const Ogre::Vector3& position, PlayerID player_id) :
Being(position, 100, player_id) {
    if(!mResourceInitialized) {
        InitializeResources();
    }
    if(mScene) {
        mMeshComponent = new dt::MeshComponent("main.mesh", "", "");
        mNode->SetScale(30);
        mNode->SetPosition(position);
        mNode->AddComponent(mMeshComponent);
    }
}

BeingPointer Building::CreateBuilding(const Ogre::Vector3& position, BuildingType type, PlayerID player_id) {
//     if (type == BuildingType::MAIN) {
    switch(type) {
        case BuildingType::MAIN: 
            BeingPointer building_ptr = BeingPointer(new Building(position, player_id));
            mBeingMap.insert({building_ptr->GetBeingID(), building_ptr});
            return building_ptr;
        
    };
    BeingPointer building_ptr = BeingPointer(new Building(position, player_id));
    mBeingMap.insert({building_ptr->GetBeingID(), building_ptr});
    return building_ptr;
}

void Building::MoveTo(const Ogre::Vector3& position) {
    Being::MoveTo(position);
}

void Building::InitializeResources()
{
    if(!mResourceInitialized) {
    Being::InitializeResources();
    dt::ResourceManager::Get()->AddResourceLocation("main","FileSystem");
    mResourceInitialized = true; 
    }
}

void Building::OnUpdateStart(double time_diff) {
}

void Building::OnUpdateEnd(double time_diff) {
}

