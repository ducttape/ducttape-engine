#include "GuiManager.hpp"

#include "DisplayManager.hpp"

namespace dt {

GuiManager::GuiManager() {
    mPlatform = nullptr;
    mGuiSystem = nullptr;
}

void GuiManager::Initialize() {
    if(mGuiSystem == nullptr) {
        Ogre::SceneManager* scene_mgr = DisplayManager::Get()->GetMainCamera()->GetCamera()->getSceneManager();

        mPlatform = new MyGUI::OgrePlatform();
        mPlatform->initialise(DisplayManager::Get()->GetRenderWindow(), scene_mgr, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        mGuiSystem = new MyGUI::Gui();
        mGuiSystem->initialise();

        EventManager::Get()->AddListener(this);

    }
}

void GuiManager::Deinitialize() {
    if(mGuiSystem != nullptr) {
        EventManager::Get()->RemoveListener(this);

        mGuiSystem->shutdown();
        delete mGuiSystem;
        mGuiSystem = nullptr;

        mPlatform->shutdown();
        delete mPlatform;
        mPlatform = nullptr;
    }
}

void GuiManager::HandleEvent(std::shared_ptr<Event> e) {
    // inject the events into the gui system

    if(e->GetType() == "DT_MOUSEEVENT") {
        std::shared_ptr<MouseEvent> m = std::dynamic_pointer_cast<MouseEvent>(e);
        if(m->GetAction() ==  MouseEvent::MOVED) {
            mGuiSystem->injectMouseMove(m->GetMouseState().X.abs, m->GetMouseState().Y.abs, m->GetMouseState().Z.abs);
        } else if(m->GetAction() ==  MouseEvent::PRESSED) {
            mGuiSystem->injectMousePress(m->GetMouseState().X.abs, m->GetMouseState().Y.abs, MyGUI::MouseButton::Enum(m->GetButton()));
        } else if(m->GetAction() ==  MouseEvent::RELEASED) {
            mGuiSystem->injectMouseRelease(m->GetMouseState().X.abs, m->GetMouseState().Y.abs, MyGUI::MouseButton::Enum(m->GetButton()));
        }
    } else if(e->GetType() == "DT_KEYBOARDEVENT") {
        std::shared_ptr<KeyboardEvent> k = std::dynamic_pointer_cast<KeyboardEvent>(e);
        if(k->GetAction() == KeyboardEvent::PRESSED) {
            mGuiSystem->injectKeyPress(MyGUI::KeyCode::Enum(k->GetCode()), k->GetText());
        } else if(k->GetAction() == KeyboardEvent::RELEASED) {
            mGuiSystem->injectKeyRelease(MyGUI::KeyCode::Enum(k->GetCode()));
        }
    }
}

void GuiManager::SetSceneManager(Ogre::SceneManager* scene_manager) {
    if(mPlatform != nullptr) {
        mPlatform->getRenderManagerPtr()->setSceneManager(scene_manager);
    }
}

MyGUI::Gui* GuiManager::GetGuiSystem() {
    // initialize if not already happened
    Initialize();
    return mGuiSystem;
}

GuiManager* GuiManager::Get() {
    return DisplayManager::Get()->GetGuiManager();
}

}
