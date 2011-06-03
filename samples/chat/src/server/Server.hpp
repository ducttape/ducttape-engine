#ifndef DUCTTAPE_SAMPLE_CHAT_SERVER_SERVER
#define DUCTTAPE_SAMPLE_CHAT_SERVER_SERVER

#include "game/Game.hpp"

class Server : public dt::Game, public dt::EventListener {
public:
    void OnInitialize();
    void HandleEvent(dt::Event* e);

};

#endif
