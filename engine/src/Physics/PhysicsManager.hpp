
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_PHYSICS_PHYSICSMANAGER
#define DUCTTAPE_ENGINE_PHYSICS_PHYSICSMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
//#include <Event/Event.hpp>
//#include <Event/EventListener.hpp>
#include <Physics/PhysicsWorld.hpp>

#include <boost/ptr_container/ptr_map.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
  * A manager for keeping the physics world and for taking care of the complicated initialization.
  */
class DUCTTAPE_API PhysicsManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    PhysicsManager();

    void Initialize();
    void Deinitialize();
    int GetEventPriority() const;

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static PhysicsManager* Get();

    /**
      * Returns whether a world with the given name exists.
      * @param name The name of the world to look for.
      * @returns Whether a world with the given name exists.
      */
    bool HasWorld(const QString& name);

    /**
      * Adds a PhysicsWorld.
      * @param world The new PhysicsWorld.
      * @returns A pointer to the new PhysicsWorld.
      */
    PhysicsWorld* AddWorld(PhysicsWorld* world);

    /**
      * Gets a PhysicsWorld.
      * @param name The name of the PhysicsWorld to find.
      * @returns A pointer to the PhysicsWorld, or nullptr of none was found.
      */
    PhysicsWorld* GetWorld(const QString& name);

public slots:
    void UpdateFrame(double simulation_frame_time);

private:
    boost::ptr_map<QString, PhysicsWorld> mWorlds;  //!< The list of PhysicsWorlds.
};

}

#endif
