
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Random.hpp>

#include "PlayerCPU.hpp"

#include "Soldier.hpp"

PlayerCPU::PlayerCPU() :
last_action_time(0),
turn_passed(false),
mGold(1000) {}

void PlayerCPU::OnUpdate(double time_diff) {
    mTotalTime += time_diff;
    _MakeDecision();
}

void PlayerCPU::_MakeDecision() {
    turn_passed = (mTotalTime - last_action_time) > 1;
    bool a = mGold > 100u;
    if(a && turn_passed) {
        last_action_time = mTotalTime;
        Ogre::Vector3 soldier_pos = mPosCQ;
        soldier_pos.z += 100 + dt::Random::Get(1,10) * 10;;
        soldier_pos.x += dt::Random::Get(1,10) * 10;
        Soldier::CreateSoldier(soldier_pos, mPlayerID);
        mGold -= 500;
        turn_passed = false;
        RaiseGold(1);
    }
}
