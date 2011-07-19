
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_INPUTMANAGER
#define DUCTTAPE_ENGINE_INPUT_INPUTMANAGER

#include <OIS.h>
#include <OgreWindowEventUtilities.h>

#include "utils/Logger.hpp"
#include "utils/Utils.hpp"
#include "MouseEvent.hpp"
#include "KeyboardEvent.hpp"
#include "event/WindowClosedEvent.hpp"

namespace dt {

/**
  * Used to capture all OIS input events and forward them to the Event system, and also for unbuffered input.
  */
class InputManager : public OIS::KeyListener,
        public OIS::MouseListener,
        public Ogre::WindowEventListener {
public:
    /**
      * Default constructor.
      */
    InputManager();

    /**
      * Initializator.
      */
    void Initialize(Ogre::RenderWindow* window);

    /**
      * Deinitializator.
      */
    void Deinitialize();

    /**
      * Captures the input devices and calls all events.
      */
    void Capture();

    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent& event);
    virtual bool keyReleased(const OIS::KeyEvent& event);

    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent& event);
    virtual bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID button);
    virtual bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID button);

    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* window);
    virtual void windowClosed(Ogre::RenderWindow* window);

private:
    Ogre::RenderWindow* mWindow;
    OIS::InputManager* mInputSystem;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;

};

}

#endif
