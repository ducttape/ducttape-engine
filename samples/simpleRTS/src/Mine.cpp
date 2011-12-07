
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Mine.hpp"

std::vector<BeingID> Mine::mMinesIDs; 

Mine::Mine(const Ogre::Vector3& position, PlayerID player_id) : 
AnimableBeing(position, 1, player_id),
mGold(1000) {
    mMinesIDs.push_back(mBeingID);
}

const std::vector<BeingID>& Mine::GetMinesIDs() {
    return mMinesIDs;
}


void Mine::Dig(PlayerID player_id) {
    if(mGold > 0) {
        Player* player = Player::FindPlayer(player_id);
        if(player) {
            player->RaiseGold(50);
        }
    } else {
        emit Depleted();
        mMinesIDs.erase(std::find(mMinesIDs.begin(), mMinesIDs.end(), mBeingID));
        Destroy();
    }
}

MinePointer Mine::CreateMine(const Ogre::Vector3& position) {
    MinePointer mine_ptr(new Mine(position, 0));
    mBeingMap.insert(std::make_pair(mine_ptr->GetBeingID(), mine_ptr));
    return mine_ptr;
}

