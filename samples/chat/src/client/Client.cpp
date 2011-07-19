
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Client.hpp"
#include "Client.hpp"

#include "ChatMessageEvent.hpp"

Client::Client() {
    mServerIP = sf::IpAddress::LocalHost;
}

void Client::OnInitialize() {
    dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);
    dt::Logger::Get().GetStream("debug")->SetDisabled(true);
    dt::Logger::Get().GetStream("info")->SetDisabled(true);

    std::shared_ptr<dt::NetworkEvent> ptr(new ChatMessageEvent("",""));
    dt::Root::get_mutable_instance().GetNetworkManager()->RegisterNetworkEventPrototype(ptr);

    dt::Root::get_mutable_instance().GetNetworkManager()->BindSocket();
    dt::Root::get_mutable_instance().GetNetworkManager()->Connect(dt::Connection(mServerIP, 29876));

    mInputThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Client::InputThread, this));
    mInputThread->Launch();
}

void Client::HandleEvent(std::shared_ptr<dt::Event> e) {
    Game::HandleEvent(e);

    if(e->GetType() == "CHATMESSAGEEVENT") {
        std::shared_ptr<ChatMessageEvent> c = std::dynamic_pointer_cast<ChatMessageEvent>(e);
        if(c->IsLocalEvent()) { // we just received this
            std::cout << std::endl << "<" << c->GetSenderNick() << "> " << c->GetMessageEvent() << std::endl;
        }
    }
}

void Client::SetServerIP(sf::IpAddress server_ip) {
    mServerIP = server_ip;
}

sf::IpAddress Client::GetServerIP() const {
    return mServerIP;
}

void Client::SetNick(const std::string& nick) {
    mNick = nick;
}

const std::string& Client::GetNick() const {
    return mNick;
}

void Client::InputThread(void* user_data) {
    Client* client = (Client*)user_data;

    while(true) {
        std::string in;
        std::getline(std::cin, in);

        if(in.substr(0, 6) == "/nick ") {
            std::string nick = in.substr(6);
            client->SetNick(nick);
            std::cout << "** You changed your nick to: " << nick << std::endl;
        } else if(in.substr(0,5) == "/ping") {
            std::cout << "** Your ping is: " << dt::Root::get_mutable_instance().GetNetworkManager()->GetConnectionsManager()->GetPing(1) << std::endl;
        } else if(in == "/quit" || in == "/exit") {
            client->RequestShutdown();
        } else {
            dt::Root::get_mutable_instance().GetEventManager()->InjectEvent(new ChatMessageEvent(in, client->GetNick()));
        }
    }
}
