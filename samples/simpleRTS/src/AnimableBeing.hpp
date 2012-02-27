
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef ANIMABLEBEING_HPP
#define ANIMABLEBEING_HPP

#include "Being.hpp"

/**
 * AnimableBeing is able to handle animation.
 * Being that has animation (like attacking, walking) must inherit from 
 * this class.
 */ 

class AnimableBeing : public Being {
    Q_OBJECT
public:
    /**
     * @param position The position where the AnimableBeing will be placed.
     * @param player_id The id of the player that owns this AnimableBeing.
     */
    AnimableBeing(const Ogre::Vector3& position, uint32_t hp, PlayerID player_id);
    
    virtual ~AnimableBeing();
    
    /**
     * Set an animation.
     * @param lenght the lenght of the animation in seconds.
     */
    void SetAnimation(float lenght, const QString& animation_name);
    
    virtual void OnUpdateStart(double time_diff);
    virtual void OnUpdate(double time_diff);
    virtual void OnUpdateEnd(double time_diff);
    
    /**
     * Called when the animation is ended.
     */
    virtual void StopAnimation();
    
protected:
//     BeingPointer mAimedBeing; //!< The being wich is subjected to the animation 
//                               //!< (e.g. an attacked being).
    
private:
    float mAnimationLenght; //!< The lenght of the animation in seconds.
    double mAnimationStart; //!< Time when the animation started in seconds.
    
};


#endif // ANIMABLEBEING_HPP
