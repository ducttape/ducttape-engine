
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
          mStart(Ogre::Vector3(0.0f, 0.0f, 0.0f)),
          mEnd(Ogre::Vector3(0.0f, 0.0f, 0.0f)) {}
}