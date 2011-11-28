#include <Config.hpp>

#include <Logic/AdvancePlayerComponent.hpp>

#include <QString>

/**
  * A sample fps controller.
  */
class FPSPlayerComponent : public dt::AdvancePlayerComponent {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @param weapon_number The number of the weapons.
      * @see Component
      */
    FPSPlayerComponent(const QString& name = "");

    void OnCreate();

    void OnDestroy();

public slots:
    void ChangeWeapon(const OIS::KeyEvent& event);
private:
    QString *mWeapons;
};