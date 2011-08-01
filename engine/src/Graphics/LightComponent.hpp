
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <OgreColourValue.h>
#include <OgreLight.h>

#include <memory>
#include <string>

namespace dt {

    /**
      * The light component.
      * @see Component
      */
    class DUCTTAPE_API LightComponent : public Component {
    public:
        /**
          * Advanced constructor.
          * @param name The name for this component.
          * @see Component
          */
        LightComponent(const std::string& name = "");

        virtual void HandleEvent(std::shared_ptr<Event> e);

        /**
          * Called when the color of the light is changed.
          */
        virtual void OnChangeColor();

        void OnCreate();
        void OnDestroy();
        void OnEnable();
        void OnDisable();
        void OnUpdate(double time_diff);

        /**
          * Sets the color of the light.
          * @param color The color of the light.
          */
        void SetColor(const Ogre::ColourValue color);

        /**
          * Get the ogre light object.
          * @return A pointer to the ogre light object.
          */
        const Ogre::Light* GetOgreLight() const;

    protected:
        Ogre::Light* mLight;    //!< The pointer to the ogre light object.
    };
}

#endif
