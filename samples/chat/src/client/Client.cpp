
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
#include <Network/NetworkManager.hpp>
#include <Network/ConnectionsManager.hpp>
#include <Scene/StateManager.hpp>

Client::Client() {
    mServerIP = sf::IpAddress::LocalHost;
}

void Client::onInitialize() {
    QObject::connect((QObject*)dt::NetworkManager::get(), SIGNAL(newEvent(std::shared_ptr<dt::NetworkEvent>)),
                      this,                               SLOT(_handleEvent(std::shared_ptr<dt::NetworkEvent>)));
    dt::Logger::get().getStream("debug")->setDisabled(true);
    dt::Logger::get().getStream("info")->setDisabled(true);

    std::shared_ptr<dt::NetworkEvent> ptr(new ChatMessageEvent("",""));
    dt::NetworkManager::get()->registerNetworkEventPrototype(ptr);

    dt::NetworkManager::get()->bindSocket();
    dt::NetworkManager::get()->connect(dt::Connection(mServerIP, 29876));

    mInputThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Client::inputThread, this));
    mInputThread->launch();
}

void Client::updateStateFrame(double simulation_frame_time) {
    dt::NetworkManager::get()->handleIncomingEvents();
    dt::NetworkManager::get()->sendQueuedEvents();
}

void Client::_handleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    if(e->getType() == "CHATMESSAGEEVENT") {
        std::shared_ptr<ChatMessageEvent> c = std::dynamic_pointer_cast<ChatMessageEvent>(e);
        //if(c->IsLocalEvent()) { // we just received this
            std::cout << std::endl << "<" << dt::Utils::toStdString(c->getSenderNick()) << "> " <<
                         dt::Utils::toStdString(c->getMessageText()) << std::endl;
        //}
    }
}

void Client::setServerIP(sf::IpAddress server_ip) {
    mServerIP = server_ip;
}

sf::IpAddress Client::getServerIP() const {
    return mServerIP;
}

void Client::setNick(const QString nick) {
    mNick = nick;
}

const QString Client::getNick() const {
    return mNick;
}

void Client::inputThread(void* user_data) {
    Client* client = (Client*)user_data;

    while(true) {
        std::string in;
        std::getline(std::cin, in);

        if(in.substr(0, 6) == "/nick ") {
            QString nick = QString(in.substr(6).c_str());
            client->setNick(nick);
            std::cout << "** You changed your nick to: " << dt::Utils::toStdString(nick) << std::endl;
        } else if(in.substr(0,5) == "/ping") {
            std::cout << "** Your ping is: " << dt::ConnectionsManager::get()->getPing(1) << std::endl;
        } else if(in == "/quit" || in == "/exit") {
            // quit this state. the application will terminate
            dt::StateManager::get()->pop();
        } else {
            dt::NetworkManager::get()->queueEvent(std::make_shared<ChatMessageEvent>(QString(in.c_str()), client->getNick()));
        }
    }
}
