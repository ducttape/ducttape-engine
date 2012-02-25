
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef CROWD_H
#define CROWD_H

#include "Being.hpp"

#include <Scene/Node.hpp>

#include <OgreVector3.h>

#include <vector>
#include <stdint.h>

class Crowd : QObject {
Q_OBJECT
public:
    Crowd(Ogre::Vector3 center = Ogre::Vector3::ZERO);
    virtual ~Crowd();
    
    /**
     * Add a being to the crowd.
     * @param being
     */
    void AddBeing(BeingPointer being);
    
    /**
     * Add a deque of being to the crowd.
     * @param beings
     */
    void AddBeing(std::deque<BeingPointer> beings);
    

    
    /**
     * Get the number of node in the crowd.
     * @return the number of node in the crowd.
     */
    uint64_t GetNodeNumber();
    
    /**
     * Place the elements in formation.
     * @todo add enum for formation type.
     */
    void PlaceFormation();
    
    /**
     * Move the crowd to position.
     * @param position the point where to move the crowd.
     */
    void MoveTo(const Ogre::Vector3& position);
    
    /**
     * Roretate each being in the Crowd to face a direction.
     * @param position the point where the beings will face.
     */
    void LookAt(const Ogre::Vector3& position);
    
    /**
     * Roteate the entire crowd to face a direction.
     * @param position the position where the crowd will face.
     */
    void RoteateTo(const Ogre::Vector3& position);
    
    /**
     * Destroy all the beings in the crowd.
     */
    void DestroyBeings();
    
public slots:
  
    /**
     * Remove a being.
     */
    void RemoveBeing(BeingID being_id);
    
private:
    Ogre::Vector3 mCenter; //! Center of the crowd.
    std::map<BeingID, BeingPointer> mBeings; //! Elements of the crowd.
    
};

#endif // CROWD_H
