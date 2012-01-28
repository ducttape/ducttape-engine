
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_AGENTCOMPONENT_H
#define DUCTTAPE_ENGINE_COMPONENT_AGENTCOMPONENT_H

#include <Logic/NavigationManager.hpp>

#include <Scene/Component.hpp>
#include <DetourCrowd.h>

namespace dt {
    
class Crowd;

class DUCTTAPE_API AgentComponent : public Component {
// Q_OBJECT

public:
    ~AgentComponent();
    AgentComponent(dt::Crowd* crowd, int32_t index, const QString& name = "");
    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);
    
    /**
     * Move to a position.
     * @param position the position where the agent will move.
     */
    void MoveTo(const Ogre::Vector3& position);
    
private:
    dtCrowdAgent* mAgent; //!< The detour agent.
    dt::Crowd* mCrowd; //!< The crowd this agent belongs to.
    int32_t mIndex; //!< Index of the agent in the crowd.
    
};

}

#endif // DUCTTAPE_ENGINE_COMPONENT_AGENTCOMPONENT_H
