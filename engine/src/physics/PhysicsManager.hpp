
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_PHYSICS_PHYSICSMANAGER
#define DUCTTAPE_ENGINE_PHYSICS_PHYSICSMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include "Config.hpp"
#include "Manager.hpp"
#include "PhysicsWorld.hpp"
#include "event/EventListener.hpp"

namespace dt {

/**
  * A manager for keeping the physics world and for taking care of the complicated initialization.
  */
class DUCTTAPE_API PhysicsManager : public Manager, public EventListener {
public:
    /**
      * Default constructor.
      */
    PhysicsManager();

    void Initialize();
    void Deinitialize();
    void HandleEvent(std::shared_ptr<dt::Event> e);
    Priority GetEventPriority() const;

    static PhysicsManager* Get();

    bool HasWorld(const std::string& name);
    PhysicsWorld* AddWorld(PhysicsWorld* world);
    PhysicsWorld* GetWorld(const std::string& name);

private:
    boost::ptr_map<std::string, PhysicsWorld> mWorlds;

};

}

#endif
