
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef MOVABLEBEING_HPP
#define MOVABLEBEING_HPP

#include "AnimableBeing.hpp"

/** 
 * A MovableBeing is a being able to move.
 * Civilians, Soldiers and veichles are MovableBeing.
 */
class MovableBeing : public AnimableBeing {
    Q_OBJECT
    
public:    
    
    /**
     * Move the MovableBeing to a specific position.
     * It's virtual because each MovableBeing may implement it's moving
     * process (e.g. path, flight, etc.)
     * @param position the position where the MovableBeing will move to.
     */
    virtual void MoveTo(Ogre::Vector3 position);
    
    /**
     */
    virtual void StopMoving();
/*    
signals:
    void Moved();*/
    /** 
     * Wheter the MovableBeing is moving 
     */
    bool IsMoving(); 
    


protected:
    
    
    MovableBeing(const Ogre::Vector3& position, uint32_t hp, PlayerID player_id);
    virtual ~MovableBeing();
    
    /**
     * Update the MovableBeing during the game. 
     * This method is called by the static method Being::UpdateBeings. 
     * @param time_diff time of the frame.
     */
    virtual void OnUpdate(double time_diff);
    
    /**
     * The MovableBeing is not moving anymore.
     */
    virtual void EndedMoving() = 0;
    
    /**
     * The path which the Being is following.
     * It's used for basic movement, it could be also ignored by the user.
     */
    dt::FollowPathComponent* mPath;
    float mSpeed;               //!< Speed of the MovableBeing's movement in units per seconds.
    double mHP;                            //!< Hit Points.
    Ogre::Vector3 MovingToPosition; //!< The position where the being is going to.
    bool Moving; //!< Wheter the MovableBeing is moving 
    

private:
    void OnUpdateStart(double time_diff);
    void OnUpdateEnd(double time_diff);
};

#endif // MOVABLEBEING_HPP
