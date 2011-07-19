
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT
#define DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT

#include "game/Game.hpp"

#include <SFML/System/Thread.hpp>
#include <memory>

class Client : public dt::Game, public dt::EventListener {
public:
    Client();

    void OnInitialize();
    void HandleEvent(std::shared_ptr<dt::Event> e);

    void SetServerIP(sf::IpAddress server_ip);
    sf::IpAddress GetServerIP() const;

    void SetNick(const std::string& nick);
    const std::string& GetNick() const;

    static void InputThread(void* user_data);
private:
    std::shared_ptr<sf::Thread> mInputThread;
    std::string mNick;
    sf::IpAddress mServerIP;
};

#endif
