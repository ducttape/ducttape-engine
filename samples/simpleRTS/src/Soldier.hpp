
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include "GameMap.hpp"
#include "MovableBeing.hpp"
#include "Mine.hpp"

class Soldier;

typedef std::shared_ptr<Soldier> SoldierPointer;

class Soldier : public MovableBeing {
    Q_OBJECT

public:
    Soldier(const Ogre::Vector3& position, PlayerID player_id);
    virtual ~Soldier();
    
    /**
     * Create Soldiers.
     * @param position Where the soldier will be created.
     * @param number of Soldiers that will be created.
     * @param player_id the player ID that will own the soldiers.
     * @return a deque containing the BeingID of all the created soldiers.
     */
    static SoldierPointer CreateSoldier(const Ogre::Vector3& position, PlayerID player_id);
    
    /**
     * Move the Soldier to a specific position.
     * The Soldier will be animated during the movement
     * @param position the position where the Soldier will move to.
     */
    virtual void MoveTo(const Ogre::Vector3& position);
    

    /**
     * Attack a being.
     * @param being_ptr The being id of the being to attack. 
     */
    void AttackBeing(const BeingPointer& being_ptr);

    
    /**
     * Update the Soldier during the game. 
     * This method is called by the static method Being::UpdateBeings. 
     * @param time_diff time of the frame.
     */
    virtual void OnUpdate(double time_diff);
    
    virtual void StopMoving();
    
    virtual void StopAnimation();
    
public slots: 
    
    /**
     * Stop attacking, remove attacked's pointer
     */
    void StopAttacking();
    
        
    void StopMining();
        
private:
    void InitializeResources();
    
        
    /**
     * Search a near enemy being and attack it.
     */ 
    void _SearchAndAttackEnemy();
    
    /**
     * Search a near mine and start digging it.
     */ 
    void _SearchAndDigMine();
    
    /**
     * Dig the mine.
     */
    void _Dig();
    
    /**
     * Set a period of pause. The Soldier will not execute the main
     * loop during the pause, he will only be animated.
     * @param lenght the lenght of the pause in seconds.
     */
    void _SetPause(float lenght);

    
    void EndedMoving();
    
    void _Attack();
    
    static bool mResourceInitialized;
    
    float mAttackPower; //!< The HP that will be subtracted from the attacked. 
    float mAttackRange; //!< The maximum distance between attacker and attacked. 
    
    bool mIsAttacking; //!< Wheter the Attacker is attacking.
    bool mAggressive; //!< Wheter the Attacker is aggressive or neutral.
    
    BeingPointer mAttackedBeing; //!< The being that is being attacked.

        bool mIsMining; //!< Wheter the MinerDelegate is mining.
    bool mIsSearchingMine; //!< Wheter the MinerDelegate is searching for a mine.

    MinePointer mMine; //!< The mine that is being digged.
    
    float mPauseLenght; //!< The lenght of the pause in seconds.
    double mPauseStart; //!< Time when the pause started in seconds.

//     double mHP;                            //!< Hit Points. 
};

#endif // SOLDIER_HPP
