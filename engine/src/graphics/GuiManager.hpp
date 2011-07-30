
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GRAPHICS_GUIMANAGER
#define DUCTTAPE_ENGINE_GRAPHICS_GUIMANAGER

#include <cstdint>
#include <string>
#include <map>

#define MYGUI_DONT_USE_OBSOLETE
#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#include "Config.hpp"
#include "event/EventManager.hpp"
#include "input/MouseEvent.hpp"
#include "input/KeyboardEvent.hpp"
#include "Manager.hpp"

namespace dt {

class DUCTTAPE_API GuiManager : public Manager, public EventListener {
public:
    GuiManager();

    void Initialize();
    void Deinitialize();
    void HandleEvent(std::shared_ptr<Event> e);

    void SetSceneManager(Ogre::SceneManager* scene_manager);

    MyGUI::Gui* GetGuiSystem();

    /**
      * Only for internal use. Sets the visibility of the mouse cursor.
      * @see void InputManager::SetMouseCursorMode(MouseCursorMode mode);
      * @param visible Whether the mouse cursor should be visible.
      * @internal
      */
    void SetMouseCursorVisible(bool visible);

    static GuiManager* Get();

private:
    MyGUI::Gui* mGuiSystem;
    MyGUI::OgrePlatform* mPlatform;
    bool mMouseCursorVisible;

};

}

#endif
