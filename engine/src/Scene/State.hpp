
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_STATES_STATE
#define DUCTTAPE_ENGINE_STATES_STATE

#include <Config.hpp>

//#include <Event/Event.hpp>
//#include <Event/EventListener.hpp>
#include <Scene/Scene.hpp>

#include <QObject>
#include <QString>

#include <memory>

namespace dt {

/**
  * A class DUCTTAPE_API for managing different stages of menu and gameplay.
  * @warning Class mockup only. Documentation suspended.
  * @see StateManager
  */
class DUCTTAPE_API State : public QObject {
    Q_OBJECT

public:
    /**
      * Default constructor.
      */
    State();

    /**
      * Called when the State is initialized. Create the scene(s) and its contents here.
      */
    virtual void onInitialize() = 0;

    /**
      * Called when the State is deinitialized.
      */
    virtual void onDeinitialize();

    /**
      * Initializes the State.
      */
    void initialize();

    /**
      * Deinitializes the state.
      */
    void deinitialize();

    /**
      * Adds a scene to the state.
      * @param scene The scene to add.
      * @returns A pointer to the scene.
      */
    Scene::SceneSP addScene(Scene* scene);

    /**
      * Returns the scene with the given name.
      * @param name The name of the Scene to find.
      * @returns The scene, or nullptr if it was not found.
      */
    Scene::SceneSP getScene(const QString name);

    /**
      * Deletes a scene.
      * @param name The name of the Scene to delete.
      */
    void deleteScene(const QString name);

    /**
      * Manually updates every scene
      * @param simulation_frame_time time since last update
      */
    void updateSceneFrame(double simulation_frame_time);

    /**
      * State update function to be defined in children
      * @param simulation_frame_time time since last update
      */
    virtual void updateStateFrame(double simulation_frame_time) = 0;

public slots:
    void updateFrame(double simulation_frame_time);

private:
    std::map<QString, Scene::SceneSP> mScenes;        //!< List of scenes.

};

} // namespace dt

#endif
