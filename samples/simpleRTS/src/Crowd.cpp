
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Crowd.hpp"
#include "RTSUtils.hpp"

Crowd::Crowd(Ogre::Vector3 center) :
mCenter(center) {}

Crowd::~Crowd() {}

void Crowd::AddBeing(BeingPointer being) {
    QObject::connect(dynamic_cast<Being*>(being.get()), SIGNAL(killed(BeingID)), this, SLOT(RemoveBeing(BeingID)));
    mBeings.insert({being->GetBeingID(), being});
}

void Crowd::AddBeing(std::deque<BeingPointer> beings)
{
//     mBeings.reserve(mBeings.size() + distance(beings.begin(), beings.end()));
//     mBeings.insert(mBeings.end(), beings.begin(), beings.end());
}

void Crowd::RemoveBeing(BeingID being_id) {
    mBeings.erase(being_id);
}

uint64_t Crowd::GetNodeNumber() {
    return mBeings.size();
}

void Crowd::MoveTo(const Ogre::Vector3& position)
{
    for(std::map<BeingID, BeingPointer>::iterator id = mBeings.begin(); id!=mBeings.end(); ++id) {
        Being* being = id->second.get();
        if(being) {
            Ogre::Vector3 distance =  being->GetNode()->GetPosition() - mCenter; // evaluate distance from center.
            distance.y = 0; //TODO: update when terrain is ready.
            being->MoveTo(position + distance); // mantain distance from center.
        }
    }
    mCenter = position;
}

void Crowd::PlaceFormation()
{
    uint64_t node_number = mBeings.size();
    uint8_t rows = 4;
    uint8_t columns = node_number / rows;
    columns += 1;
    
    // Assume all node have same dimension. TODO: adjust for right dimension.
    Ogre::Vector3 size;
Being* being = mBeings.begin()->second.get();
if(being) size = being->GetBoundingBox().getSize();
    
    float padding = 20;
    size.x += padding;
    size.z += padding;
    
    float formation_width = size.x * columns;
    float formation_depth = size.z * rows; 
    
    // Evaluate top-left corner, then place element from left to right, from up to down.
    Ogre::Vector3 corner = mCenter - Ogre::Vector3(formation_width/2, 0, formation_depth/2);
    uint8_t col, row;
    col = row = 0;
    
    for(std::map<BeingID, BeingPointer>::iterator id = mBeings.begin(); id!=mBeings.end(); ++id) {
        Being* being = id->second.get();
        if(being) {
            col = col % columns;
            float half_width = size.x / 2;
            float half_depth = size.z / 2;
            // Evaluate the position where the being will be moved.
            Ogre::Vector3 position(corner.x + half_width + size.x * col, 
                                mCenter.y, 
                                corner.z + half_depth + size.z * row); 
            being->MoveTo(position);
            
            // Place the next being in the next column.
            ++col;
            if(col == columns) {
                // New row.
                col = 0;
                ++row;
            }
        }
    }
}

void Crowd::LookAt(const Ogre::Vector3& position) {
    for(std::map<BeingID, BeingPointer>::iterator id = mBeings.begin(); id!=mBeings.end(); ++id) {
        Being* being = id->second.get();
        if(being) {
            being->LookAt(position);
        }
    }
}

void Crowd::RoteateTo(const Ogre::Vector3& position) {
    for(std::map<BeingID, BeingPointer>::iterator id = mBeings.begin(); id!=mBeings.end(); ++id) {
        Being* being = id->second.get();
        if(being) {
            Ogre::Vector3 being_position = being->GetNode()->GetPosition();
            Ogre::Vector3 direction = position - mCenter;
            float angle = Ogre::Vector3::UNIT_Z.getRotationTo(direction, 
                                                    Ogre::Vector3::UNIT_X).getYaw().valueRadians();
            // the front of the formation should face the position, so add 90°
            angle += Ogre::Math::PI / 2; 
            Ogre::Vector3 final = RTSUtils::Rotation2D(being_position, mCenter, angle);
            being->MoveTo(final);
            being->LookAt(position);
        }
    }
}

void Crowd::DestroyBeings() {
    for(std::map<BeingID, BeingPointer>::iterator id = mBeings.begin(); id!=mBeings.end(); ++id) {
        id->second->Destroy();
    }
}


