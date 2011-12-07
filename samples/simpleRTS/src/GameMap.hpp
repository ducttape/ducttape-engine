
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "Being.hpp"
// #include "Mine.hpp"

#include <OgreVector2.h>

#include <boost/numeric/ublas/matrix.hpp>
#include "Player.hpp"
class Being;
class Mine;

typedef uint32_t BeingID;

typedef std::tr1::shared_ptr<Being> BeingPointer;
typedef std::tr1::shared_ptr<Mine> MinePointer;

typedef std::list<uint32_t> BeingList;
typedef std::vector<BeingList> Vector;
typedef std::map<BeingID, BeingPointer> MapCell;

class GameMap
{

public:
   
   static GameMap* GetGameMapInstance();
   
//    AddBeing(BeingID being_ID);
//    UpdateBeingPosition(BeingID being_ID, const Ogre::Vector2& position);
   
   static void UpdateMap(double time_diff);
   
   static BeingPointer GetNearEnemyBeing(Being* being);
   static BeingPointer GetNearEnemyBeing(const BeingID being_ID);
   
   static MapCell* GetBeingListIn(float x, float y);
   static MapCell* GetBeingListIn(const Ogre::Vector2& position);
   
   static MinePointer GetNearMine(Being* being);
   
private:
    
    GameMap();
    
    static GameMap* mSingletonInstance;
    
//     static float mRemainingTime;
    static std::map<BeingID, BeingPointer> mMatrix[200][200];
//     std::map<BeingID, Cell*> mBeingCellMap; //!< 

    
    static void _VectorToCell(const Ogre::Vector2& vector, uint8_t& x, uint8_t& y);
};

#endif // GAMEMAP_HPP
