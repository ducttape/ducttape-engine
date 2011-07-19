
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_FOLLOWPATHCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_FOLLOWPATHCOMPONENT

#include <string>

#include <OgreLight.h>

#include "component/Component.hpp"

namespace dt {

/**
  * Lets the node follow a bezier path.
  * @todo extrude the path stuff to make it usable somewhere else as well
  */
class FollowPathComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      * @param name The name of the component.
      */
    FollowPathComponent(const std::string& name);

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(float time_diff);

    void AddPoint(Ogre::Vector3 point);
    /**
      * Sets the speed with which the Node should travel along the path.
      * @param speed The speed in units per second.
      */
    void SetSpeed(float speed);

    /**
      * Sets the duration the path should take to complete.
      * @param duration The duration in seconds.
      */
    void SetDuration(float duration);

    /**
      * Returns the total length of all path segments.
      * @returns The total length of all path segments.
      */
    float GetTotalLength() const;

    /**
      * Resets the node to the beginning of the path.
      */
    void Reset();

protected:
    /**
      * Calculates the position for the current progress.
      * @returns The position the node should have right now.
      */
    Ogre::Vector3 _CalculatePosition();

private:
    std::vector<Ogre::Vector3> mPoints;
    float mDurationSinceStart;
    float mTotalDuration;

};

}

#endif
