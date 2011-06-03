#include "Client.hpp"

#include "ChatMessageEvent.hpp"

Client::Client() {
    mServerIP = sf::IpAddress::LocalHost;
}

void Client::OnInitialize() {
    dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

    dt::Root::get_mutable_instance().GetNetworkManager()->RegisterNetworkEventPrototype(new ChatMessageEvent("",""));
    dt::Root::get_mutable_instance().GetNetworkManager()->BindSocket();
    dt::Root::get_mutable_instance().GetNetworkManager()->Connect(dt::Connection(mServerIP, 29876));

    mInputThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Client::InputThread, this));
    mInputThread->Launch();
}

void Client::HandleEvent(dt::Event* e) {
    if(e->GetType() == "CHATMESSAGEEVENT") {
        ChatMessageEvent* c = (ChatMessageEvent*)e;
        if(c->IsLocalEvent()) { // we just received this
            std::cout << std::endl << "<" << c->GetSenderNick() << "> " << c->GetMessage() << std::endl;
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
        std::cin >> in;

        if(in.substr(0, 6) == "/nick ") {
            std::string nick = in.substr(6);
            client->SetNick(nick);
            std::cout << "** You changed your nick to: " << nick << std::endl;
        } else if(in == "/quit" || in == "/exit") {
            client->RequestShutdown();
        } else {
            dt::Root::get_mutable_instance().GetEventManager()->HandleEvent(new ChatMessageEvent(in, client->GetNick()));
        }
    }
}
