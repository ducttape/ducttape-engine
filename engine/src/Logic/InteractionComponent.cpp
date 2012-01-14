
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/InteractionComponent.hpp>

#include <BtOgreGP.h>

namespace dt {
    InteractionComponent::InteractionComponent(const QString& name)
        : Component(name),
          mRange(0.0f),
          mOffset(0.0f),
          mInterval(1.0f),
          mRemainTime(0.0f) {}

    void InteractionComponent::SetRange(float range) {
        mRange = range;
    }

    void InteractionComponent::SetOffset(float offset) {
        mOffset = offset;
    }

    float InteractionComponent::GetRange() const {
        return mRange;
    }

    float InteractionComponent::GetOffset() const {
        return mOffset;
    }

    float InteractionComponent::GetIntervalTime() const {
        return mInterval;
    }

    void InteractionComponent::SetIntervalTime(float interval_time) {
        mInterval = interval_time;
    }

    float InteractionComponent::GetRemainTime() const {
        return mRemainTime;
    }

    void InteractionComponent::SetRemainTime(float remain_time) {
        mRemainTime = remain_time;
    }

    void InteractionComponent::Check() {
        if(IsReady() && this->IsEnabled()) {
            mRemainTime = mInterval;
            Ogre::Vector3 start, end;
            start = GetNode()->GetRotation(Node::SCENE) * Ogre::Vector3(0.0, 0.0, - mOffset)
                + GetNode()->GetPosition(Node::SCENE);
            end = GetNode()->GetRotation(Node::SCENE) * Ogre::Vector3(0.0, 0.0, - mRange)
                + GetNode()->GetPosition(Node::SCENE);


            emit sCheck(start, end);
            OnCheck(BtOgre::Convert::toBullet(start), BtOgre::Convert::toBullet(end));
        }
    }

    void InteractionComponent::OnUpdate(double time_diff) {
        if(!IsReady()) {
            mRemainTime -= time_diff;
        }
    }

    bool InteractionComponent::IsReady() const {
        return mRemainTime <= 0.0f;
    }
}