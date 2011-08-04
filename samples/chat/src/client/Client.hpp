
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT
#define DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT

#include <Scene/Game.hpp>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Thread.hpp>

#include <memory>

class Client : public dt::State {
public:
    Client();

    void OnInitialize();
    void HandleEvent(std::shared_ptr<dt::Event> e);

    void SetServerIP(sf::IpAddress server_ip);
    sf::IpAddress GetServerIP() const;

    void SetNick(const QString& nick);
    const QString& GetNick() const;

    static void InputThread(void* user_data);
private:
    std::shared_ptr<sf::Thread> mInputThread;
    QString mNick;
    sf::IpAddress mServerIP;
};

#endif
