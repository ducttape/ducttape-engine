
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_GUNCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_GUNCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>
#include <Logic/InteractionComponent.hpp>

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include <QString>

namespace dt {

/**
  * A component for interacting with other objects in the scene.
  */
class DUCTTAPE_API RaycastComponent : public InteractionComponent {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    RaycastComponent(const QString& name = "");

    virtual void OnCreate();
    virtual void OnDestroy();

    /**
      * Called when the ray hits an object. It will emit a hit signal. You can just connect it with a slot or just override it.
      * @param hit The hit object.
      */
    virtual void OnHit(Node* hit);

    /*
     * Called when Check() is called. It will emit a hit signal. You can just connect it with a slot or just override it.
     */
    virtual void OnCheck();

    /**
      * Check if there's any objects hit by the ray.
      * @see InteractionComponent
      */
    void Check();

signals:
    void sHit(Node* hit);
    void sCheck(Ogre::Vector3 start, Ogre::Vector3 end);

private:
    btCollisionWorld::ClosestRayResultCallback* mRaycastCallback;    //<! The raycast callback.
};

}

#endif