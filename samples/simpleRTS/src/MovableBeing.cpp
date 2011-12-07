
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MovableBeing.hpp"

MovableBeing::MovableBeing(const Ogre::Vector3& position, uint32_t hp, PlayerID player_id) :
AnimableBeing(position, hp, player_id),
mPath(nullptr),
mSpeed(100), 
MovingToPosition(position),
Moving(true) {}


void MovableBeing::MoveTo(Ogre::Vector3 position) {
//     //abort old path.
//     StopMoving();
//     
//     // assume same height. TODO: update when field is ready.
//     float distance = position.distance(mNode->GetPosition());
//     float travel_time = distance / mSpeed;
//     
//     mPath = mNode->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::SINGLE, "path"));
//     mPath->SetFollowRotation(false);
//     mPath->SetSmoothAcceleration(true);
//     mPath->SetSmoothCorners(true);
//     mPath->AddPoint(mNode->GetPosition());
//     mPath->AddPoint(position);
//     mPath->SetDuration(travel_time);
    MovingToPosition = position;
    Moving = true;
}

void MovableBeing::StopMoving() {
//     if(mPath) {
//         mNode->RemoveComponent("path");
//         mPath = nullptr;
//     }
    Moving = false;
}

bool MovableBeing::IsMoving() {
    return Moving;
}


MovableBeing::~MovableBeing() {}


void MovableBeing::OnUpdate(double time_diff) {
    if(mLookingAt || mStaringAtBeing) {
        Ogre::Quaternion current = mNode->GetRotation();
        Ogre::Vector3 direction;
        if(mLookingAt) {
            direction = mLookAtPosition - mNode->GetPosition(dt::Node::PARENT);
        } else if(mStaringAtBeing) {
            direction = mStareAtBeing->GetNode()->GetPosition() - mNode->GetPosition(dt::Node::PARENT);
        }
        Ogre::Quaternion end = Ogre::Vector3::UNIT_Z.getRotationTo(direction, 
                                                                  Ogre::Vector3::UNIT_X);
        Ogre::Quaternion applied;
        static Ogre::Real phase = 0;
        phase += time_diff / 10000;
        applied = Ogre::Quaternion::nlerp(phase, current, end, true);
        mNode->SetRotation(applied);
        if(phase>1) {
            mLookingAt = false;
            phase = 0; // TODO: possible error when staring at a being, if angle is resetted verify this.
        }
    }
}

void MovableBeing::OnUpdateStart(double time_diff) {
    
}


void MovableBeing::OnUpdateEnd(double time_diff) {
                // calculate new position
    if(Moving) {
        Ogre::Vector2 position;
        Ogre::Vector3 end_position; 
        position.x = (MovingToPosition - mNode->GetPosition()).x;
        position.y = (MovingToPosition - mNode->GetPosition()).z;
        position.normalise();
        end_position.x = position.x;
        end_position.y = 0;
        end_position.z = position.y;
        end_position = mNode->GetPosition() + end_position / 100;
        if(end_position == MovingToPosition) {
            Moving = false;
            EndedMoving();
        }
        mNode->SetPosition(end_position);
        UpdateCell();
    }
}
