
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef MINE_HPP
#define MINE_HPP

#include "Player.hpp"
#include "Being.hpp"
#include "AnimableBeing.hpp"

class Mine : public AnimableBeing {
Q_OBJECT    
public:
    /**
     */
    Mine(const Ogre::Vector3& position, PlayerID player_id);
    
    void Dig(PlayerID player_id);
    
    static const std::vector<BeingID>& GetMinesIDs();
     
    static MinePointer CreateMine(const Ogre::Vector3& position); 
    
signals:
    /**
     * Emitted when mGold goes under zero.
     */ 
    void Depleted();
    

    
private:
    static std::vector<BeingID> mMinesIDs; //!< List of the BeingID of the not Depleted mines. 
    uint32_t mGold; //!< The gold unit remaining in this mine.
};

typedef std::shared_ptr<Mine> MinePointer;

#endif // MINE_HPP
