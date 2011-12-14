#include <Config.hpp>

#include <Logic/AdvancePlayerComponent.hpp>

#include <boost/ptr_container/ptr_map.hpp>

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
    FPSPlayerComponent(int weapon_num, const QString& name = "");

    //void AddWeapon(IWeapon* weapon);

    void RemoveWeapon(int type);

    void OnCreate();

    void OnDestroy();

public slots:
    void ChangeWeapon(const OIS::KeyEvent& event);
private:
    QString *mWeapons;
    int mWeaponNum;
};