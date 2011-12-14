#pragma once

#include <Config.hpp>

#include <QString>

/**
  * An interface for weapon.
  */
class IWeapon {
public:
    /**
      * Gets the type of the weapon.
      * @returns The type of the weapon.
      */
    virtual int GetWeaponType() = 0;   
};