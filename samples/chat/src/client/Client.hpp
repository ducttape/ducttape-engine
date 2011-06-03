#ifndef DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT
#define DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT

#include "game/Game.hpp"

#include <SFML/System/Thread.hpp>
#include <memory>

class Client : public dt::Game, public dt::EventListener {
public:
    void OnInitialize();
    void HandleEvent(dt::Event* e);

    void SetNick(const std::string& nick);
    const std::string& GetNick() const;

    static void InputThread(void* user_data);
private:
    std::shared_ptr<sf::Thread> mInputThread;
    std::string mNick;
};

#endif
