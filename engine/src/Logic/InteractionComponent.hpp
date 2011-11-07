
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_INTERACTIONCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_INTERACTIONCOMPONENT

#include <Config.hpp>

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
      */
    virtual void Check() = 0;

protected:
    Ogre::Vector3 mStart;          //<! The start position of the InteractionComponent's range relative to its position.
    Ogre::Vector3 mEnd;            //<! The end position of the InteractionComponent's range relative to its position.
};

}

#endif