
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Client.hpp"

#include "ChatMessageEvent.hpp"

#include <Core/Root.hpp>
#include <Utils/Utils.hpp>
#include <Event/EventManager.hpp>
#include <Network/NetworkManager.hpp>
#include <Network/ConnectionsManager.hpp>
#include <Scene/StateManager.hpp>

Client::Client() {
    mServerIP = sf::IpAddress::LocalHost;
}

void Client::OnInitialize() {
    dt::EventManager::Get()->AddListener(this);
    dt::Logger::Get().GetStream("debug")->SetDisabled(true);
    dt::Logger::Get().GetStream("info")->SetDisabled(true);

    std::shared_ptr<dt::NetworkEvent> ptr(new ChatMessageEvent("",""));
    dt::NetworkManager::Get()->RegisterNetworkEventPrototype(ptr);

    dt::NetworkManager::Get()->BindSocket();
    dt::NetworkManager::Get()->Connect(dt::Connection(mServerIP, 29876));

    mInputThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Client::InputThread, this));
    mInputThread->Launch();
}

void Client::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "CHATMESSAGEEVENT") {
        std::shared_ptr<ChatMessageEvent> c = std::dynamic_pointer_cast<ChatMessageEvent>(e);
        if(c->IsLocalEvent()) { // we just received this
            std::cout << std::endl << "<" << dt::Utils::ToStdString(c->GetSenderNick()) << "> " << dt::Utils::ToStdString(c->GetMessageText()) << std::endl;
        }
    }
}

void Client::SetServerIP(sf::IpAddress server_ip) {
    mServerIP = server_ip;
}

sf::IpAddress Client::GetServerIP() const {
    return mServerIP;
}

void Client::SetNick(const QString& nick) {
    mNick = nick;
}

const QString& Client::GetNick() const {
    return mNick;
}

void Client::InputThread(void* user_data) {
    Client* client = (Client*)user_data;

    while(true) {
        std::string in;
        std::getline(std::cin, in);

        if(in.substr(0, 6) == "/nick ") {
            QString nick = QString(in.substr(6).c_str());
            client->SetNick(nick);
            std::cout << "** You changed your nick to: " << dt::Utils::ToStdString(nick) << std::endl;
        } else if(in.substr(0,5) == "/ping") {
            std::cout << "** Your ping is: " << dt::ConnectionsManager::Get()->GetPing(1) << std::endl;
        } else if(in == "/quit" || in == "/exit") {
            // quit this state. the application will terminate
            dt::StateManager::Get()->Pop();
        } else {
            dt::EventManager::Get()->
                InjectEvent(std::make_shared<ChatMessageEvent>(QString(in.c_str()), client->GetNick()));
        }
    }
}
