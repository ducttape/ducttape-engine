
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Player.hpp"
#include "Building.hpp"

uint8_t Player::mPlayerCount = 0; 
std::map<PlayerID, Player*>  Player::mPlayerList; 

Player::Player() {
     mPlayerID = mPlayerCount++;
     mPlayerList.insert({mPlayerID, this});
     mPosCQ = Ogre::Vector3(200*mPlayerID, 0, 200*mPlayerID);
     Building::CreateBuilding(mPosCQ, BuildingType::MAIN, mPlayerID);
}

Player::~Player() {

}

void Player::UpdatePlayers(double time_diff) {
    for(std::map<PlayerID, Player*>::iterator it = mPlayerList.begin(); it != mPlayerList.end(); ++it) {
        it->second->OnUpdate(time_diff);
    }
}

Player* Player::FindPlayer(PlayerID player_id) {
   std::map<PlayerID, Player*>::iterator it = mPlayerList.find(player_id);
   if(it == mPlayerList.end()) {
       return nullptr;
   }
   return it->second;
}

void Player::RaiseGold(uint32_t gold) {
    mGold += gold;
}
