
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "GameMap.hpp"
#include "Mine.hpp"

#include <float.h>

GameMap* GameMap::mSingletonInstance = nullptr;
MapCell GameMap::mMatrix[200][200];

 MapCell* GameMap::GetBeingListIn(float x, float y) {
    Ogre::Vector2 position;
    position.x = x;
    position.y = y;
    return GetBeingListIn(position);
}

 MapCell* GameMap::GetBeingListIn(const Ogre::Vector2& position) {
    uint8_t x, y;
    _VectorToCell(position, x, y);
    return  &mMatrix[x][y];
}


GameMap::GameMap() {}

GameMap* GameMap::GetGameMapInstance() {
    if(mSingletonInstance) {
        mSingletonInstance = new GameMap;
    }
    return mSingletonInstance;
}

// GameMap::AddBeing(BeingID being_ID) {
//     std::map<BeingID, Cell*>::iterator it = mBeingCellMap.find();
//     if(it == mBeingCellMap.end()) {
//         Being* being = Being::FindBeing(being_ID);
//         being->Get2DPosition();
//     }
// }


void GameMap::UpdateMap(double time_diff) {
//     static uint8_t x, y = 0;
// //     for(int x = 0; x < 100; ++x) {
// //         for (int y = 0; y < 100; ++y) {
// //             mMatrix[x][y].clear();
// //         }
// //     }
// // dt::Logger::Get().Debug("matrix [" + QString::number(x) + "] [" + QString::number(y) + "]" );   
// 
//     while(y<99) { 
//         mMatrix[x][y].clear();
//         ++y;
//     } 
//         y=0;
//         if(x<99) {
//             ++x;
//             return;
//         } else {
//             x=0;
//         }
// 
//     
//     std::map<BeingID, BeingPointer> all_being = Being::GetAllBeings();
//     for(std::map<BeingID, BeingPointer>::iterator it = all_being.begin(); it != all_being.end(); ++it) {
//         BeingPointer being = it->second;
//         Ogre::Vector2 pos = being->Get2DPosition();
//         uint8_t x, y;
//         _VectorToCell(pos, x, y);
//         mMatrix[x][y].insert(being);
// dt::Logger::Get().Debug("added Being n." + QString::number(being->GetBeingID()) + " " + QString::number(x) + " " + QString::number(y));
//     }
}


BeingPointer GameMap::GetNearEnemyBeing(Being* being) {

        //TODO Cell doesn't overlap, so two near being may be on different cell.
        // Solve this with two overlapping matrix?
    PlayerID player_id = being->GetPlayerID();
    MapCell* map_cell = being->GetMapCell();
    for(MapCell::iterator it = map_cell->begin(); it != map_cell->end(); ++it) {
        BeingPointer enemy_being = it->second;
        if(player_id != enemy_being->GetPlayerID()) {
            return enemy_being;
        }
    }
     return BeingPointer();
}

BeingPointer GameMap::GetNearEnemyBeing(const BeingID being_ID) {
    BeingPointer being_ptr = Being::FindBeing(being_ID);
    return GetNearEnemyBeing(being_ptr.get());
}

MinePointer GameMap::GetNearMine(Being* being) {
    static double distance = DBL_MAX;
    BeingPointer return_mine = BeingPointer();
    Ogre::Vector3 being_position = being->GetNode()->GetPosition();
    std::vector<BeingID> mine_vector = Mine::GetMinesIDs();

    for(std::vector<BeingID>::iterator it = mine_vector.begin(); it != mine_vector.end(); ++it) {
        BeingPointer mine = Being::FindBeing(*it); 
        
        if(mine->GetNode()->GetPosition().squaredDistance(being_position) > distance) {
            return_mine = mine;
        }
    }
    
    return  std::static_pointer_cast<Mine>(return_mine); //TODO correct this
}



inline void GameMap::_VectorToCell(const Ogre::Vector2& vector, uint8_t& x, uint8_t& y) {
    x = vector.x / 1000 + 100;
    y = vector.y / 1000 + 100;
    if(x >= 200 || x <= 0) {
        x = 100;
    }
    if(x >= 100 || x <= 0) {
        x = 100;
    }
}
