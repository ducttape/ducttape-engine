
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef PLAYERCPU_HPP
#define PLAYERCPU_HPP

#include "Player.hpp"

class PlayerCPU : public Player
{

public:
    PlayerCPU();
    
    /**
     * Update the PlayerCPU during the game. 
     * This method is called by the static method Player::UpdatePlayers. 
     * Here the CPU takes decisions.
     * @param time_diff time of the frame.
     */
    virtual void OnUpdate(double time_diff);
    
    
    
private:
    double mTotalTime; //!< Time from the creation of the Player.
    double last_action_time;
    bool turn_passed;
    void _MakeDecision();
    uint64_t mGold; 
};

#endif // PLAYERCPU_HPP
