
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/InteractionComponent.hpp>

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
        if(mRemainTime <= 0.0f) {
            mRemainTime = mInterval;
            OnCheck();
        }
    }

    void InteractionComponent::OnUpdate(double time_diff) {
        if(mRemainTime > 0.0f) {
            mRemainTime -= time_diff;
        }
    }
}