
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

void Server::OnInitialize() {
    connect((QObject*)dt::NetworkManager::Get(), SIGNAL(NewEvent(std::shared_ptr<dt::NetworkEvent>)),
        this, SLOT(_HandleEvent(std::shared_ptr<dt::NetworkEvent>)));


    std::shared_ptr<dt::NetworkEvent> ptr(new ChatMessageEvent("",""));
    dt::NetworkManager::Get()->RegisterNetworkEventPrototype(ptr);

    dt::NetworkManager::Get()->BindSocket(29876);
}

void Server::UpdateStateFrame(double simulation_frame_time) {
    dt::NetworkManager::Get()->HandleIncomingEvents();
    dt::NetworkManager::Get()->SendQueuedEvents();
}

void Server::_HandleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    // This is quite useful for debugging purposes.
    //dt::Logger::Get().Info("There are " + boost::lexical_cast<QString>(dt::ConnectionsManager::Get()->GetConnectionCount()) + " connections active.");

    if(e->GetType() == "CHATMESSAGEEVENT") {
        std::shared_ptr<ChatMessageEvent> c = std::dynamic_pointer_cast<ChatMessageEvent>(e);

        //if(c->IsLocalEvent()) { // we just received this

            if(c->GetMessageText() == "/help") {
                QString msg = "\nThe following commands are available:\n\
                              /help - This message\n\
                              /quit - disconnects from the server\n\
                              /nick [nickname] - changes your nickname";
                dt::NetworkManager::Get()->
                    QueueEvent(std::make_shared<ChatMessageEvent>(msg, c->GetSenderNick()));
            } else {
                std::cout << std::endl << dt::Utils::ToStdString(c->GetSenderNick()) << ": "
                    << dt::Utils::ToStdString(c->GetMessageText()) << std::endl;
            }

            // send back to everyone else
            dt::NetworkManager::Get()->
                QueueEvent(std::make_shared<ChatMessageEvent>(c->GetMessageText(),
                c->GetSenderNick()));
        //}

    } else if(e->GetType() == "DT_GOODBYEEVENT") {
        dt::Logger::Get().Info("Client disconnected: " +
            std::dynamic_pointer_cast<dt::GoodbyeEvent>(e)->GetReason());
    }
}
