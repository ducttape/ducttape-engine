
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GRAPHICS_VIEWPORT
#define DUCTTAPE_ENGINE_GRAPHICS_VIEWPORT

#include <memory>
#include <string>

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include "Config.hpp"
#include "component/CameraComponent.hpp"
#include "utils/Logger.hpp"

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


    void setCamera(Ogre::Camera* cam);

    void setBackgroundColour(const Ogre::ColourValue& color);

    /**
      * Hides mViewport from being visible on screen.
      */
    void hide();

    /**
      * Restores its size to mViewport making it visible again.
      */
    void show();

private:

    Ogre::Viewport* mViewport;
    bool hidden;
    Ogre::Real top;
    Ogre::Real left;
    Ogre::Real height;
    Ogre::Real width;
};

}

#endif
