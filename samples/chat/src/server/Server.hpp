
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_SAMPLE_CHAT_SERVER_SERVER
#define DUCTTAPE_SAMPLE_CHAT_SERVER_SERVER

#include "game/Game.hpp"

class Server : public dt::Game, public dt::EventListener {
public:
    void OnInitialize();
    void HandleEvent(dt::Event* e);

};

#endif
