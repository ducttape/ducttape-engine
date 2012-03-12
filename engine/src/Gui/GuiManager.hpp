
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIMANAGER
#define DUCTTAPE_ENGINE_GUI_GUIMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Gui/GuiRootWindow.hpp>
#include <Input/InputManager.hpp>

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#include <OgreSceneManager.h>

#include <memory>


namespace dt {

/**
  * Manager class for the GUI System.
  * @see http://mygui.info
  */
class DUCTTAPE_API GuiManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    GuiManager();

    void initialize();
    void deinitialize();

    /**
      * Sets the scene manager to use for the GUI display.
      * @param scene_manager The scene manager to use.
      */
    void setSceneManager(Ogre::SceneManager* scene_manager);

    /**
      * Returns MyGUI's GUI system.
      * @returns The GUI System.
      */
    MyGUI::Gui* getGuiSystem();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static GuiManager* get();

    GuiRootWindow& getRootWindow();

public slots:
    /**
      * Only for internal use. Sets the visibility of the mouse cursor.
      * @see void InputManager::SetMouseCursorMode(MouseCursorMode mode);
      * @param visible Whether the mouse cursor should be visible.
      * @internal
      */
    void setMouseCursorVisible(bool visible);

    void sPressed(dt::InputManager::InputCode input_code, const OIS::EventArg& event);
    void sReleased(dt::InputManager::InputCode input_code, const OIS::EventArg& event);
    void sMouseMoved(const OIS::MouseEvent& event);

signals:
    void mouseCursorVisibilityChanged(bool visible);

private:
    bool _eventEnabled();              //!< Whether the MyGUI event system is enabled.
    MyGUI::Gui* mGuiSystem;            //!< MyGUI's GUI system.
    MyGUI::OgrePlatform* mPlatform;    //!< MyGUI's OgrePlatform.
    bool mMouseCursorVisible;          //!< Whether the GUI mouse cursor is visible.
    GuiRootWindow mRootGuiWindow;      //!< The root window widget.
    Ogre::SceneManager* mSceneManager; //!< The scene manager used for the GUI.
    MyGUI::InputManager* mMyguiInputMgrPtr;
};

}

#endif
