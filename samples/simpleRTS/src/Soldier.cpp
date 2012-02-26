
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Soldier.hpp"

#include <deque>

#include <Core/ResourceManager.hpp>
#include "Mine.hpp"

bool Soldier::mResourceInitialized = false;

Soldier::Soldier(const Ogre::Vector3& position, PlayerID player_id) : 
MovableBeing(position, 100, player_id),
mIsAttacking(false),
mAggressive(true),
mIsMining(false),
mIsSearchingMine(false) {
    if(!mResourceInitialized) {
        InitializeResources();
    }
    if(mScene) {
        mMeshComponent = new dt::MeshComponent("Sinbad.mesh", "", "");
        mNode->AddComponent(mMeshComponent);
    }
}

Soldier::~Soldier() {
}

void Soldier::InitializeResources() {
    if(!mResourceInitialized) {
    Being::InitializeResources();
    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    mResourceInitialized = true; 
    }
}

void Soldier::_SearchAndAttackEnemy() {
    BeingPointer being_ptr = GameMap::GetNearEnemyBeing(this);
    if(being_ptr.get() != nullptr) {
        AttackBeing(being_ptr);
    }
}


SoldierPointer Soldier::CreateSoldier(const Ogre::Vector3& position, PlayerID player_id) {
    SoldierPointer soldier_ptr(new Soldier(position, player_id));
    mBeingMap.insert(std::make_pair(soldier_ptr->GetBeingID(), soldier_ptr));
    return soldier_ptr;
}

void Soldier::MoveTo(const Ogre::Vector3& position) {
    // TODO: stop all the animation or concurrent animation possible? 
    if(!IsMoving()) {
        SetAnimation(9999, "RunBase"); //TODO default mode: infinite animation.
    }
    MovableBeing::MoveTo(position);
}

void Soldier::EndedMoving() {
//    mMeshComponent->GetOgreEntity()->getAnimationState("RunBase")->setEnabled(false);  
//    mMeshComponent->StopAnimation();
}

void Soldier::OnUpdate(double time_diff)
{
    MovableBeing::OnUpdate(time_diff);
    AnimableBeing::OnUpdate(time_diff);
    if(GetNowTime() - mPauseStart > mPauseLenght) {
        if(mIsAttacking) {
    //TODO: possible ottimization here, check if the being is able to move or it moved.
            Ogre::Vector3 being_position = mAttackedBeing->GetNode()->GetPosition();
            float distance = being_position.squaredDistance(mNode->GetPosition());
            if(distance>100) {
                MoveTo(being_position);
            } else {
                StopMoving();
                _Attack();
            }
        } else if(mAggressive) {
            _SearchAndAttackEnemy();
        } else if (mIsSearchingMine) {
            _SearchAndDigMine();
        } else if (mIsMining) {
            _Dig();
        }
    }
}

void Soldier::StopMoving() {
    MovableBeing::StopMoving();
//     mMeshComponent->StopAnimation();
}


void Soldier::StopAnimation() {

}

void Soldier::_Attack() {
    SetAnimation(1,QString("SliceHorizontal")); 
    _SetPause(1);
    mAttackedBeing->ReduceHP(100);
}

void Soldier::_Dig() {
 mMine->Dig(mPlayerID);
}

void Soldier::_SetPause(float lenght) {
    mPauseLenght = lenght;
    mPauseStart = GetNowTime();
}

void Soldier::_SearchAndDigMine() {
    MinePointer mine_ptr = GameMap::GetNearMine(this);
    if(mine_ptr.get() != nullptr) {
        mIsMining = true;
        mMine = mine_ptr;
    }
}

void Soldier::AttackBeing(const BeingPointer& being_ptr) {
    QObject::connect(being_ptr.get(), SIGNAL(killed(BeingID)), this, SLOT(StopAttacking()));
    mAttackedBeing = being_ptr;
    mIsAttacking = true;
    StareAtBeing(mAttackedBeing);
}

void Soldier::StopAttacking() {
    mAttackedBeing.reset();
    mIsAttacking = false;
//     StopAnimation(); TODO:When the Soldier is doing nothing, assure that he has no animation. 
    StopStareAtBeing(); 
}

void Soldier::StopMining() {
    mIsMining = false;
}
