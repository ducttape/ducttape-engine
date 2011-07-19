
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ---------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT

#include <string>

#include <OgreLight.h>
#include <OgreColourValue.h>
#include <OgreVector3.h>
#include <OgreMath.h>

#include "component/Component.hpp"

namespace dt {

    /**
      * The light component.
      * @see Component
      */
    class LightComponent : public Component {
    public:
        /**
          * Advanced constructor.
          * @see Component
          */
        LightComponent(const std::string& name = "");

        virtual void HandleEvent(std::shared_ptr<Event> e){}
        /**
          * Called when the color of the light is changed.
          */
        virtual void OnChangeColor(){}

        /**
          * Override the OnCreate of the Component.
          * @see Component
          */
        void OnCreate();

        /**
          * Override the OnDestroy of the Component.
          * @see Component
          */
        void OnDestroy();

        /**
          * Override the OnEnable of the Component. Used to turn on the light(set it visible).
          * @see Component
          */
        void OnEnable();

        /**
          * Override the OnDisable of the Component. Used to turn off the light(set it unvisible).
          * @see Component
          */
        void OnDisable();

        /**
          * Override the OnUpdate of the Component.
          * @see Component
          */
        void OnUpdate(double time_diff);

        /**
          * Set the color of the light.
          * @param the color of the light
          */
        void SetColor(const Ogre::ColourValue color);

        /**
          * Get the ogre light object.
          * @return A pointer to the ogre light object.
          */
        const Ogre::Light* GetOgreLight() const;

    protected:
        Ogre::Light* mLight;                   //!< The pointer to the ogre light object.
    };
}

#endif