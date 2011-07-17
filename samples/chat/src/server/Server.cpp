
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Server.hpp"

#include "ChatMessageEvent.hpp"

void Server::OnInitialize() {
    dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

    dt::Root::get_mutable_instance().GetNetworkManager()->RegisterNetworkEventPrototype(new ChatMessageEvent("",""));
    dt::Root::get_mutable_instance().GetNetworkManager()->BindSocket(29876);
}

void Server::HandleEvent(dt::Event* e) {
    // This is quite useful for debugging purposes.
    //dt::Logger::Get().Info("There are " + boost::lexical_cast<std::string>(dt::Root::get_mutable_instance().GetNetworkManager()->GetConnectionsManager()->GetConnectionCount()) + " connections active.");

    if(e->GetType() == "CHATMESSAGEEVENT") {
        ChatMessageEvent* c = (ChatMessageEvent*)e;

        if(c->IsLocalEvent()) { // we just received this

            if(c->GetMessageEvent() == "/help") {
                std::string msg = "\nThe following commands are available:\n    /help - This message\n    /quit - disconnects from the server\n    /nick [nickname] - changes your nickname";
                dt::Root::get_mutable_instance().GetEventManager()->HandleEvent(new ChatMessageEvent(msg, c->GetSenderNick()));
            } else {
                std::cout << std::endl << c->GetSenderNick() << ": " << c->GetMessageEvent() << std::endl;
            }

            // send back to everyone else
            dt::Root::get_mutable_instance().GetEventManager()->HandleEvent(new ChatMessageEvent(c->GetMessageEvent(), c->GetSenderNick()));
        }

    } else if(e->GetType() == "DT_GOODBYEEVENT") {
        dt::Logger::Get().Info("Client disconnected: " + ((dt::GoodbyeEvent*)e)->GetReason());
    }
}
