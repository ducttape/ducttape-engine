
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

#include <QString>

#include <memory>

namespace dt {

/**
  * The light component.
  * @see Component
  */
class DUCTTAPE_API LightComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name for this component.
      * @see Component
      */
    LightComponent(const QString& name = "");

    /**
      * Called when the color of the light is changed.
      */
    virtual void OnColorChanged();

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

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

public slots:
    /**
      * Sets the color of the light.
      * @param color The color of the light.
      */
    void SetColor(const Ogre::ColourValue color);

signals:
    void ColorChanged(const Ogre::ColourValue new_color);

protected:
    Ogre::Light* mLight;    //!< The pointer to the ogre light object.
    Ogre::SceneNode* mSceneNode; //!< The pointer to the ogre SceneNode the light is attached to.
    bool mCastShadows;              //!< Whether the light should cast shadows.
};

}

#endif
