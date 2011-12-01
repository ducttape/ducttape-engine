#include <Config.hpp>

#include <Logic/CollisionComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

#include <QString>

/**
  * A component representing the fast weapon.
  */
class SlowWeaponComponent : public dt::CollisionComponent {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    SlowWeaponComponent(const QString& bullet_handle, const QString& name = "");

    /**
      * Sets the weapon's power.
      * @param power The power to set.
      */
    void SetPower(float power);

    /**
      * Gets the weapon's power.
      * @returns The weapon's power.
      */
    float GetPower();

private:
    float mPower;
};