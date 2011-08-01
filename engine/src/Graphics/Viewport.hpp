
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GRAPHICS_VIEWPORT
#define DUCTTAPE_ENGINE_GRAPHICS_VIEWPORT

#include <Config.hpp>

#include <OgreCamera.h>
#include <OgreColourValue.h>
#include <OgreViewport.h>

namespace dt {

/**
  * A Ogre::Viewport class wrapper.
  */
class DUCTTAPE_API Viewport {
public:
    /**
      * Default constructor.
      */
    Viewport();

    /**
      * Default destructor.
      */
    ~Viewport();

    /**
      * Initializer.
      */
    void Initialize(Ogre::Viewport* Viewport);

    /**
      * Deinitializer.
      */
    void Deinitialize();

    /**
      * Sets the camera for this viewport.
      * @param cam The camera.
      */
    void SetCamera(Ogre::Camera* cam);

    /**
      * Sets the background color for this viewport.
      * @param color The background color.
      */
    void SetBackgroundColor(const Ogre::ColourValue& color);

    /**
      * Hides the viewport.
      */
    void Hide();

    /**
      * Shows the viewport.
      */
    void Show();

private:
    Ogre::Viewport* mViewport;  //!< The Ogre::Viewport managed by this class.
    bool hidden;        //!< Whether this viewport is hidden or not.
    Ogre::Real top;     //!< The y-coordinate of the viewport.
    Ogre::Real left;    //!< The x-coordinate of the viewport.
    Ogre::Real height;  //!< The height of the viewport.
    Ogre::Real width;   //!< The width of the viewport.
};

}

#endif
