
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_SAMPLE_CHAT_SERVER_SERVER
#define DUCTTAPE_SAMPLE_CHAT_SERVER_SERVER

#include <Config.hpp>

#include <Scene/Game.hpp>
#include <Network/NetworkEvent.hpp>

class Server : public dt::State {
    Q_OBJECT
public:
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);
private slots:
    void _handleEvent(std::shared_ptr<dt::NetworkEvent> e);

};

#endif
