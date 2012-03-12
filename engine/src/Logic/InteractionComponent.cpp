
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/InteractionComponent.hpp>

#include <BtOgreGP.h>

namespace dt {
    InteractionComponent::InteractionComponent(const QString name)
        : Component(name),
          mRange(0.0f),
          mOffset(0.0f),
          mInterval(1.0f),
          mRemainTime(0.0f) {}

    void InteractionComponent::setRange(float range) {
        mRange = range;
    }

    void InteractionComponent::setOffset(float offset) {
        mOffset = offset;
    }

    float InteractionComponent::getRange() const {
        return mRange;
    }

    float InteractionComponent::getOffset() const {
        return mOffset;
    }

    float InteractionComponent::getIntervalTime() const {
        return mInterval;
    }

    void InteractionComponent::setIntervalTime(float interval_time) {
        mInterval = interval_time;
    }

    float InteractionComponent::getRemainTime() const {
        return mRemainTime;
    }

    void InteractionComponent::setRemainTime(float remain_time) {
        mRemainTime = remain_time;
    }

    void InteractionComponent::check() {
        if(isReady() && this->isEnabled()) {
            mRemainTime = mInterval;
            Ogre::Vector3 start, end;
            start = getNode()->getRotation(Node::SCENE) * Ogre::Vector3(0.0, 0.0, - mOffset)
                + getNode()->getPosition(Node::SCENE);
            end = getNode()->getRotation(Node::SCENE) * Ogre::Vector3(0.0, 0.0, - mRange)
                + getNode()->getPosition(Node::SCENE);


            emit sCheck(start, end);
            onCheck(BtOgre::Convert::toBullet(start), BtOgre::Convert::toBullet(end));
        }
    }

    void InteractionComponent::onUpdate(double time_diff) {
        if(!isReady()) {
            mRemainTime -= time_diff;
        }
    }

    bool InteractionComponent::isReady() const {
        return mRemainTime <= 0.0f;
    }
}
