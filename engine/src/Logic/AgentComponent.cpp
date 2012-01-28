
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "AgentComponent.hpp"
#include <Scene/Node.hpp>

#include <DetourNavMesh.h>

namespace dt {

AgentComponent::AgentComponent(dt::Crowd* crowd, int32_t index, const QString& name): 
Component(name),
mCrowd(crowd),
mIndex(index) { //TODO -1 check
}

AgentComponent::~AgentComponent() {

}

void AgentComponent::OnCreate() {}

void AgentComponent::OnDestroy() {
    mCrowd->GetCrowd().removeAgent(mIndex);
}

void AgentComponent::OnEnable() {}

void AgentComponent::OnDisable() {}

void AgentComponent::OnUpdate(double time_diff) {
    dt::Component::OnUpdate(time_diff);
    Ogre::Vector3 position = dt::Utils::FloatsToOgreVector3(mCrowd->GetCrowd().getAgent(mIndex)->npos);
// std::cout << position.x << " " << position.y << " " << position.z << std::endl;
    mNode->SetPosition(position, dt::Node::SCENE);
}

void AgentComponent::OnSerialize(dt::IOPacket& packet) {
    dt::Component::OnSerialize(packet);
}

void AgentComponent::MoveTo(const Ogre::Vector3& position) {
    // Find nearest point on navmesh and set move request to that location.
    dtCrowd& detour_crowd = mCrowd->GetCrowd();
    const dtQueryFilter* filter = detour_crowd.getFilter();
    const float* ext = detour_crowd.getQueryExtents();
    
    float pos[3];
    dt::Utils::OgreVector3ToFloats(position, pos);
    
    dtPolyRef target_ref;
    float target_pos[3];

    dt::NavigationManager::Get()->GetNavQuery()->findNearestPoly(pos, ext, filter, &target_ref, target_pos);
    bool successful = detour_crowd.requestMoveTarget(mIndex, target_ref, target_pos);
    if(!successful) {
        dt::Logger::Get().Debug(" movement unsuccess");
    }
}

}