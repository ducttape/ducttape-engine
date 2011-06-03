#include "Server.hpp"

#include "ChatMessageEvent.hpp"

void Server::OnInitialize() {
    dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

    dt::Root::get_mutable_instance().GetNetworkManager()->RegisterNetworkEventPrototype(new ChatMessageEvent("",""));
    dt::Root::get_mutable_instance().GetNetworkManager()->BindSocket(29876);
}

void Server::HandleEvent(dt::Event* e) {
    if(e->GetType() == "CHATMESSAGEEVENT") {
        ChatMessageEvent* c = (ChatMessageEvent*)e;

        if(c->IsLocalEvent()) { // we just received this
            std::cout << std::endl << c->GetSenderNick() << ": " << c->GetMessage() << std::endl;

            // send back to everyone else
            dt::Root::get_mutable_instance().GetEventManager()->HandleEvent(new ChatMessageEvent(c->GetMessage(), c->GetSenderNick()));
        }

    }
}
