
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_STATES_STATE
#define DUCTTAPE_ENGINE_STATES_STATE

#include <boost/ptr_container/ptr_map.hpp>

#include "Config.hpp"
#include "event/Event.hpp"
#include "event/EventListener.hpp"
#include "scene/Scene.hpp"

namespace dt {

/**
  * A class DUCTTAPE_API for managing different stages of menu and gameplay.
  * @warning Class mockup only. Documentation suspended.
  * @see StateManager
  */
class DUCTTAPE_API State : public EventListener {
public:
    /**
      * Default constructor.
      */
    State();

    void HandleEvent(std::shared_ptr<Event> e);

    /**
      * Called when the State is initialized. Create the scene(s) and its contents here.
      */
    virtual void OnInitialize() = 0;

    /**
      * Called when the State is deinitialized.
      */
    virtual void OnDeinitialize();

    /**
      * Initializes the State.
      */
    void Initialize();

    /**
      * Deinitializes the state.
      */
    void Deinitialize();

    /**
      * Adds a scene to the state.
      * @param scene The scene to add.
      * @returns A pointer to the scene.
      */
    Scene* AddScene(Scene* scene);

    /**
      * Returns the scene with the given name.
      * @param name The name of the Scene to find.
      * @returns The scene, or nullptr if it was not found.
      */
    Scene* GetScene(const std::string& name);

    /**
      * Deletes a scene.
      * @param name The name of the Scene to delete.
      */
    void DeleteScene(const std::string& name);

private:
    boost::ptr_map<std::string, Scene> mScenes;        //!< List of scenes.

};

}

#endif
