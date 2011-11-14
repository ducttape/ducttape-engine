
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
          mOffset(0.0f) {}

    void InteractionComponent::SetRange(float range) {
        mRange = range;
    }

    void InteractionComponent::SetOffset(float offset) {
        mOffset = offset;
    }

    float InteractionComponent::GetRange() {
        return mRange;
    }

    float InteractionComponent::GetOffset() {
        return mOffset;
    }
}