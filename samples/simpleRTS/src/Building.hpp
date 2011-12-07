
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <Being.hpp>

enum BuildingType {
    MAIN
};

class Building : public Being {
    Q_OBJECT

public:
    static BeingPointer CreateBuilding(const Ogre::Vector3& position, BuildingType type, PlayerID player_id); 
    void MoveTo(const Ogre::Vector3& position);
    
protected:
    static bool mResourceInitialized;
    virtual void InitializeResources();
    Building(const Ogre::Vector3& position, PlayerID player_id);
    
    virtual void OnUpdateStart(double time_diff);
    virtual void OnUpdateEnd(double time_diff);
};

#endif // BUILDING_HPP
