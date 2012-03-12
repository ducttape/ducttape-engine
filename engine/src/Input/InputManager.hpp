
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_INPUTMANAGER
#define DUCTTAPE_ENGINE_INPUT_INPUTMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>

#define OIS_DYNAMIC_LIB
#include <OIS.h>

#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

namespace dt {

/**
  * Used to capture all OIS input events and forward them to the Event system, and also for unbuffered input.
  */
class DUCTTAPE_API InputManager : public Manager,
        public OIS::KeyListener,
        public OIS::MouseListener,
        public Ogre::WindowEventListener {
    Q_OBJECT
public:
    /**
      * The mode of the mouse cursor.
      */
    enum MouseCursorMode {
        HIDDEN,     //!< Do not display the mouse cursor.
        SYSTEM,     //!< Use the system's default cursor.
        GRAPHICAL   //!< Display a graphical mouse cursor using the GuiManager.
    };

    /**
      * Default constructor.
      */
    InputManager();

    void initialize();
    void deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static InputManager* get();

    /**
      * Sets the render window.
      * @param window The render window.
      */
    void setWindow(Ogre::RenderWindow* window);

    /**
      * Captures the input devices and calls all events.
      */
    void capture();

    /**
      * Sets whether the input deviced are jailed. If true, the mouse cannot move outside the window
      * and the keyboard events are not sent to other applications. Useful to disable for debugging.
      * Default: false.
      * @param jail_input Whether to jail the input devices.
      */
    void setJailInput(bool jail_input = false);

    /**
      * Returns whether the input devices are jailed (for details on that see InputManager::SetJailInput).
      * @returns Whether to jail the input devices.
      */
    bool getJailInput() const;

    /**
      * OIS callback.
      * @internal
      * @param event The OIS event.
      */
    virtual bool keyPressed(const OIS::KeyEvent& event);

    /**
      * OIS callback.
      * @internal
      * @param event The OIS event.
      */
    virtual bool keyReleased(const OIS::KeyEvent& event);

    /**
      * OIS callback.
      * @internal
      * @param event The OIS event.
      */
    virtual bool mouseMoved(const OIS::MouseEvent& event);

    /**
      * OIS callback.
      * @internal
      * @param event The OIS event.
      */
    virtual bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID button);

    /**
      * OIS callback.
      * @internal
      * @param event The OIS event.
      */
    virtual bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID button);

    /**
      * Ogre window callback.
      * @internal
      * @param window The Ogre::RenderWindow.
      */
    virtual void windowResized(Ogre::RenderWindow* window);

    /**
      * Ogre window callback.
      * @internal
      * @param window The Ogre::RenderWindow.
      */
    virtual void windowClosed(Ogre::RenderWindow* window);

    /**
      * Returns the mouse object.
      * @returns The mouse object.
      */
    OIS::Mouse* getMouse();

    /**
      * Returns the keyboard object.
      * @returns The keyboard object.
      */
    OIS::Keyboard* getKeyboard();

    /**
      * Sets the mouse cursor mode. Available modes are:
      * - HIDDEN (no cursor)
      * - SYSTEM (show the default system cursor)
      * - GRAPHICAL (use the GUI system to show the cursor)
      * @param mode The mouse cursor mode.
      */
    void setMouseCursorMode(MouseCursorMode mode);

    /**
      * Gets the mouse cursor mode.
      * @returns The mouse cursor mode.
      */
    MouseCursorMode getMouseCursorMode() const;

signals:
    void windowClosed();
    void sKeyPressed(const OIS::KeyEvent& event);
    void sKeyReleased(const OIS::KeyEvent& event);
    void sMouseMoved(const OIS::MouseEvent& event);
    void sMousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID button);
    void sMouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID button);
private:
    /**
      * Resets the input system. Required when the settings change.
      * @see _DestroyInputSystem();
      * @see _CreateInputSystem();
      */
    void _resetInputSystem();

    /**
      * Destroys the input system.
      */
    void _destroyInputSystem();

    /**
      * Creates the input system.
      */
    void _createInputSystem();

    Ogre::RenderWindow* mWindow;        //!< The Window used for input (from the DisplayManager).
    OIS::InputManager* mInputSystem;    //!< The OIS input manager.
    OIS::Mouse* mMouse;                 //!< The mouse object.
    OIS::Keyboard* mKeyboard;           //!< The keyboard object.

    MouseCursorMode mMouseCursorMode;   //!< The mode of the mouse cursor.
    bool mJailInput;                    //!< Whether the input devices are jailed (for details on that see InputManager::SetJailInput).
};

}

#endif
