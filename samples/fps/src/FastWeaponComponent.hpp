#include <Config.hpp>

#include <Logic/RaycastComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

#include <QString>

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
      * Sets the weapon's power.
      * @param power The power to set.
      */
    void SetPower(float power);

    /**
      * Gets the weapon's power.
      * @returns The weapon's power.
      */
    float GetPower();

    void OnCreate();

private slots:
    /**
      * Called when the bullet(ray) hits an object.
      * @param hit The hit object.
      */
    void OnHit(dt::PhysicsBodyComponent* hit);

private:
    float mPower;
};