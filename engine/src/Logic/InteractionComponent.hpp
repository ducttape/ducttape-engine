
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_INTERACTIONCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_INTERACTIONCOMPONENT

#include <Config.hpp>

#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <QString>

namespace dt {

/**
  * An abstract component for interacting with other objects in the scene. Implemented by RaycastComponent.
  */
class DUCTTAPE_API InteractionComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    InteractionComponent(const QString& name = "");

    /**
      * Check if there's any objects in the InteractionComponent's range. 
      * It will emit a sCheck signal when the check starts successfully.
      */
   void Check();

    /**
      * Sets the range.
      * @param range The range to set.
      */
    void SetRange(float range);

    /**
      * Sets the offset.
      * @param offset The offset to set.
      */
    void SetOffset(float offset);

    /**
      * Gets the range.
      * @returns The range.
      */
    float GetRange() const;

    /**
      * Gets the offset.
      * @returns The offset.
      */
    float GetOffset() const;

    /**
      * Gets the interval between two interactions.
      * @returns The interval time.
      */
    float GetIntervalTime() const;

    /**
      * Sets the interval between two interactions.
      * @param interval_time The interval time to set.
      */
    void SetIntervalTime(float interval_time);

    /**
      * Gets the remain time before the next interaction can be performed.
      * @returns The remain time.
      */
    float GetRemainTime() const;

    /**
      * Sets the remain time manually.
      * @param remain_time The remain time to set.
      */
    void SetRemainTime(float remain_time);

    void OnUpdate(double time_diff);

    /**
      * Gets whether it's ready to perform the next interaction.
      * @returns Whether it's ready to perform the next interaction.
      */
    bool IsReady() const;

protected:
    /**
      * Called when Check() is called.
      * @param start The absolute starting position for the check.
      * @param end The absolute ending position for the check.
      */
    virtual void OnCheck(const btVector3& start, const btVector3& end) = 0;

signals:
    /**
      * Emits this signal when the check completes and captures an object.
      * @param hit The captured object.
      */
    void sHit(dt::PhysicsBodyComponent* hit);

    /**
      * A signal which will be emitted when the check starts.
      * @param start The absolute starting position for the check.
      * @param end The absolute ending position for the check.
      */
    void sCheck(Ogre::Vector3 start, Ogre::Vector3 end);

protected:
    float mRange;          //<! The start position of the InteractionComponent's range relative to its position.
    float mOffset;         //<! The end position of the InteractionComponent's range relative to its position.
    float mInterval;       //<! The interval between two interaction.
    float mRemainTime;     //<! The ramain waitting time before the next interaction can be performed.
};

}

#endif