#include <Config.hpp>

#include <Logic/RaycastComponent.hpp>
#include <Scene/Node.hpp>

#include <QString.h>

/**
  * A component representing the fast weapon.
  */
class FastWeaponComponent : public dt::RaycastComponent {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    FastWeaponComponent(const QString& name = "");

    /**
      * Called when the bullet(ray) hits an object.
      * @param hit The hit object.
      */
    void OnHit(dt::Node* hit);

    /*
     * Called when Check() is called.
     */
    void OnCheck();
};