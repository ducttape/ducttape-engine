
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT

#include <string>

#include <OgreLight.h>
#include <OgreColourValue.h>
#include <OgreVector3.h>
#include <OgreMath.h>

#include "Config.hpp"
#include "component/Component.hpp"

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
    Ogre::Light* GetOgreLight() const;

    /**
      * Sets whether the mesh should cast shadows. Default: true.
      * @param shadow Whether the mesh should cast shadows.
      */
    void SetCastShadows(bool cast_shadows);

    /**
      * Gets whether the mesh should cast shadows.
      * @returns Whether the mesh should cast shadows.
      */
    bool GetCastShadows() const;

protected:
    Ogre::Light* mLight;    //!< The pointer to the ogre light object.
    Ogre::SceneNode* mSceneNode; //!< The pointer to the ogre SceneNode the light is attached to.
    bool mCastShadows;              //!< Whether the light should cast shadows.
};

}

#endif
