
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <OgreVector3.h>

#include <QtCore/QObject>

#include <cstdint>
#include <map>

typedef uint8_t PlayerID;

class Player : public QObject
{
    
Q_OBJECT

public:
    Player();
    virtual ~Player();
    
    /**
     * Update all the Player during the game. 
     * This static method should be called once in each loop of the game.
     * @param time_diff time of the frame.
     */
    static void UpdatePlayers(double time_diff);
    
    /**
     * Get Player with id.
     * @param player_id ID of the player.
     */
    static Player* FindPlayer(PlayerID player_id);
    
    void RaiseGold(uint32_t gold);
    
protected:
    static uint8_t mPlayerCount;  //!< Count of the players.
    PlayerID mPlayerID;            //!< ID number of the player.
    uint64_t mGold;              //!< Gold quantity.
    Ogre::Vector3 mPosCQ;        //!< Position of the Central Quarter.
   /**
     * Update the Player during the game. 
     * This method is called by the static method UpdatePlayers. 
     * @param time_diff time of the frame.
     */
    virtual void OnUpdate(double time_diff) = 0;
    
private:
    static std::map<PlayerID, Player*> mPlayerList; 
};

#endif // PLAYER_HPP
