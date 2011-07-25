
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

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>

#include "event/EventManager.hpp"
#include "input/MouseEvent.hpp"
#include "input/KeyboardEvent.hpp"
#include "Manager.hpp"

namespace dt {

class GuiManager : public Manager, public EventListener {
public:
    GuiManager();

    void Initialize();
    void Deinitialize();
    void HandleEvent(std::shared_ptr<Event> e);

    void SetSceneManager(Ogre::SceneManager* scene_manager);

    MyGUI::Gui* GetGuiSystem();

    static GuiManager* Get();

private:
    MyGUI::Gui* mGuiSystem;
    MyGUI::OgrePlatform* mPlatform;

};

}

#endif
