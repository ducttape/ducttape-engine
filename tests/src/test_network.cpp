#define SERVER_PORT 20501
#define CLIENT_PORT 20502
#define DATA_INCREMENT 42

#include "Root.hpp"

#include "network/NetworkManager.hpp"
#include "utils/Utils.hpp"

/**
  * @file
  * A test for the Event system sending Events over network. The client connects to the server, and sends
  * a CustomNetworkEvent. The server receives that Event, adds DATA_INCREMENT to the data value and sends
  * a new CustomNetworkEvent back to the client. The client calculates the difference of the data value
  * and checks against DATA_INCREMENT.
  */

class CustomNetworkEvent : public dt::NetworkEvent {
public:
    enum Sender {
        CLIENT = 1,
        SERVER = 2
    };

    CustomNetworkEvent(int data, Sender e) {
        mData = data;
        mEnum = e;
    }

    const std::string GetType() const {
        return "CUSTOMNETWORKEVENT";
    }

    Event* Clone() const {
        return new CustomNetworkEvent(mData, mEnum);
    }

    void Serialize(dt::IOPacket& p) {
        p & mData;
        p & dt::EnumHelper(&mEnum);
    }

public:
    uint32_t mData;
    Sender mEnum;
};

class CustomServerEventManager : public dt::EventListener {
public:
    CustomServerEventManager() {
        mDataReceived = 0;
    }

    void HandleEvent(dt::Event* e) {
        if(e->GetType() == "CUSTOMNETWORKEVENT") {
            CustomNetworkEvent* c = (CustomNetworkEvent*)e;
            if(c->mEnum == CustomNetworkEvent::CLIENT) {
                std::cout << "Server: received CustomNetworkEvent" << std::endl;
                // send it back, adding 1 to the data
                dt::Root::get_mutable_instance().GetEventManager()->HandleEvent(new CustomNetworkEvent(c->mData + DATA_INCREMENT, CustomNetworkEvent::SERVER));
                mDataReceived = c->mData;
            }
        }
    }

public:
    uint32_t mDataReceived;
};

class CustomClientEventManager : public dt::EventListener {
public:
    CustomClientEventManager() {
        mDataReceived = 0;
    }

    void HandleEvent(dt::Event* e) {
        if(e->GetType() == "CUSTOMNETWORKEVENT") {
            CustomNetworkEvent* c = (CustomNetworkEvent*)e;
            if(c->mEnum == CustomNetworkEvent::SERVER) {
                std::cout << "Client: received CustomNetworkEvent" << std::endl;
                mDataReceived = c->mData;
            }
        }
    }

public:
    uint32_t mDataReceived;
};

void server() {
    dt::Root& root = dt::Root::get_mutable_instance();
    std::cout << "-- Running server" << std::endl;

    CustomServerEventManager csem;
    root.GetEventManager()->AddListener(&csem);

    dt::NetworkManager* nm = root.GetNetworkManager();
    nm->BindSocket(SERVER_PORT);
    while(csem.mDataReceived == 0) {
        nm->HandleIncomingEvents();
        nm->SendQueuedEvents();
        sf::Sleep(100);
        if(root.GetTimeSinceInitialize() > 5000) {
            std::cerr << "Server: Time out of 5 seconds reached" << std::endl;
            exit(5);
        }
    }
}

void client() {
    dt::Root& root = dt::Root::get_mutable_instance();
    std::cout << "-- Running client" << std::endl;

    dt::NetworkManager* nm = root.GetNetworkManager();
    nm->BindSocket(CLIENT_PORT);
    nm->Connect(dt::Connection(sf::IpAddress::LocalHost, SERVER_PORT));

    int data = 1337;

    CustomClientEventManager ccem;
    root.GetEventManager()->AddListener(&ccem);
    root.GetEventManager()->HandleEvent(new CustomNetworkEvent(data, CustomNetworkEvent::CLIENT));

    while(ccem.mDataReceived == 0) {
        nm->HandleIncomingEvents();
        nm->SendQueuedEvents();
        sf::Sleep(100);
        if(root.GetTimeSinceInitialize() > 5000) {
            std::cerr << "Client: Time out of 5 seconds reached" << std::endl;
            exit(5);
        }
    }

    bool correct_data = ((int)ccem.mDataReceived == data + DATA_INCREMENT);
    if(!correct_data) {
        std::cerr << "Client: Received wrong data (" + dt::tostr(ccem.mDataReceived) + " instead of " + dt::tostr(data + DATA_INCREMENT) + ")" << std::endl;
        exit(1);
    }
}

int main(int argc, char** argv) {
    std::string arg1 = (argc < 2 ? "" : argv[1]);
    if(arg1 != "server" && arg1 != "client") {
        std::cerr << "Usage: ./test_network < server | client >" << std::endl;
        return 1;
    }

    dt::Root& root = dt::Root::get_mutable_instance();
    root.Initialize();

    // register event prototype
    root.GetNetworkManager()->RegisterNetworkEventPrototype(new CustomNetworkEvent(0, CustomNetworkEvent::CLIENT));

    if(arg1 == "server") {
        server();
    } else if(arg1 == "client") {
        client();
    } else {
        std::cerr << "This should not happen." << std::endl;
        return 1;
    }

    root.Deinitialize();
    return 0;
}

