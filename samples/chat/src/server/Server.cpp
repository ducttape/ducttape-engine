
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Server.hpp"

#include <Core/Root.hpp>
#include <Network/GoodbyeEvent.hpp>
#include <Utils/Utils.hpp>
#include <Network/NetworkManager.hpp>

#include "ChatMessageEvent.hpp"

void Server::onInitialize() {
    QObject::connect((QObject*)dt::NetworkManager::get(), SIGNAL(newEvent(std::shared_ptr<dt::NetworkEvent>)),
                      this,                               SLOT(_handleEvent(std::shared_ptr<dt::NetworkEvent>)));


    std::shared_ptr<dt::NetworkEvent> ptr(new ChatMessageEvent("",""));
    dt::NetworkManager::get()->registerNetworkEventPrototype(ptr);

    dt::NetworkManager::get()->bindSocket(29876);
}

void Server::updateStateFrame(double simulation_frame_time) {
    dt::NetworkManager::get()->handleIncomingEvents();
    dt::NetworkManager::get()->sendQueuedEvents();
}

void Server::_handleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    // This is quite useful for debugging purposes.
    //dt::Logger::Get().Info("There are " + boost::lexical_cast<QString>(dt::ConnectionsManager::Get()->GetConnectionCount()) + " connections active.");

    if(e->getType() == "CHATMESSAGEEVENT") {
        std::shared_ptr<ChatMessageEvent> c = std::dynamic_pointer_cast<ChatMessageEvent>(e);

        //if(c->IsLocalEvent()) { // we just received this

            if(c->getMessageText() == "/help") {
                QString msg = "\nThe following commands are available:\n\
                              /help - This message\n\
                              /quit - disconnects from the server\n\
                              /nick [nickname] - changes your nickname";
                dt::NetworkManager::get()->
                    queueEvent(std::make_shared<ChatMessageEvent>(msg, c->getSenderNick()));
            } else {
                std::cout << std::endl << dt::Utils::toStdString(c->getSenderNick()) << ": "
                    << dt::Utils::toStdString(c->getMessageText()) << std::endl;
            }

            // send back to everyone else
            dt::NetworkManager::get()->
                queueEvent(std::make_shared<ChatMessageEvent>(c->getMessageText(),
                c->getSenderNick()));
        //}

    } else if(e->getType() == "DT_GOODBYEEVENT") {
        dt::Logger::get().Info("Client disconnected: " +
            std::dynamic_pointer_cast<dt::GoodbyeEvent>(e)->getReason());
    }
}
