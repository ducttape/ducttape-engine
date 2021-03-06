
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT
#define DUCTTAPE_SAMPLE_CHAT_CLIENT_CLIENT

#include <Config.hpp>

#include <Scene/Game.hpp>
#include <Network/NetworkEvent.hpp>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Thread.hpp>

#include <memory>

class Client : public dt::State {
    Q_OBJECT

public:
    Client();

    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

    void setServerIP(sf::IpAddress server_ip);
    sf::IpAddress getServerIP() const;

    void setNick(const QString nick);
    const QString getNick() const;

    static void inputThread(void* user_data);
private slots:
    void _handleEvent(std::shared_ptr<dt::NetworkEvent> e);
private:
    std::shared_ptr<sf::Thread> mInputThread;
    QString mNick;
    sf::IpAddress mServerIP;
};

#endif
