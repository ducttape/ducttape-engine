
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_NETWORK
#define DUCTTAPE_ENGINE_TESTS_TEST_NETWORK

#define SERVER_PORT 20501
#define CLIENT_PORT 20502
#define DATA_INCREMENT 42

#include <Core/Root.hpp>
#include <Network/NetworkManager.hpp>
#include <Utils/Utils.hpp>

/**
  * @file
  * A test for the Event system sending Events over network. The client connects to the server, and sends
  * a CustomNetworkEvent. The server receives that Event, adds DATA_INCREMENT to the data value and sends
  * a new CustomNetworkEvent back to the client. The client calculates the difference of the data value
  * and checks against DATA_INCREMENT.
  */

namespace test_network {

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

    const QString GetType() const {
        return "CUSTOMNETWORKEVENT";
    }

    std::shared_ptr<dt::Event> Clone() const {
        std::shared_ptr<dt::Event> ptr(new CustomNetworkEvent(mData, mEnum));
        return ptr;
    }

    void Serialize(dt::IOPacket& p) {
        p & mData;
        p & dt::EnumHelper(&mEnum);
    }

public:
    uint32_t mData;
    Sender mEnum;
};

class CustomServerEventListener : public dt::EventListener {
public:
    CustomServerEventListener() {
        mDataReceived = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "CUSTOMNETWORKEVENT") {
            std::shared_ptr<CustomNetworkEvent> c = std::dynamic_pointer_cast<CustomNetworkEvent>(e);
            if(c->mEnum == CustomNetworkEvent::CLIENT) {
                std::cout << "Server: received CustomNetworkEvent" << std::endl;
                // send it back, adding 1 to the data
                dt::EventManager::Get()->
                    InjectEvent(std::make_shared<CustomNetworkEvent>(c->mData + DATA_INCREMENT, CustomNetworkEvent::SERVER));
                mDataReceived = c->mData;
            }
        }
    }

public:
    uint32_t mDataReceived;
};

class CustomClientEventListener : public dt::EventListener {
public:
    CustomClientEventListener() {
        mDataReceived = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "CUSTOMNETWORKEVENT") {
            std::shared_ptr<CustomNetworkEvent> c = std::dynamic_pointer_cast<CustomNetworkEvent>(e);
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
    dt::Root& root = dt::Root::GetInstance();
    std::cout << "-- Running server" << std::endl;

    CustomServerEventListener csel;
    root.GetEventManager()->AddListener(&csel);

    dt::NetworkManager* nm = root.GetNetworkManager();
    nm->BindSocket(SERVER_PORT);
    while(csel.mDataReceived == 0) {
        nm->HandleIncomingEvents();
        nm->SendQueuedEvents();
        sf::Sleep(100);
        if(root.GetTimeSinceInitialize() > 5.0) {
            std::cerr << "Server: Time out of 5 seconds reached" << std::endl;
            exit(5);
        }
    }
}

void client() {
    dt::Root& root = dt::Root::GetInstance();
    std::cout << "-- Running client" << std::endl;

    dt::NetworkManager* nm = root.GetNetworkManager();
    nm->BindSocket(CLIENT_PORT);
    nm->Connect(dt::Connection(sf::IpAddress::LocalHost, SERVER_PORT));

    int data = 1337;

    CustomClientEventListener ccel;
    root.GetEventManager()->AddListener(&ccel);
    root.GetEventManager()->InjectEvent(std::make_shared<CustomNetworkEvent>(data, CustomNetworkEvent::CLIENT));

    while(ccel.mDataReceived == 0) {
        nm->HandleIncomingEvents();
        nm->SendQueuedEvents();
        sf::Sleep(100);
        if(root.GetTimeSinceInitialize() > 5.0) {
            std::cerr << "Client: Time out of 5 seconds reached" << std::endl;
            exit(5);
        }
    }

    bool correct_data = ((int)ccel.mDataReceived == data + DATA_INCREMENT);
    if(!correct_data) {
        std::cerr << "Client: Received wrong data (" + dt::Utils::ToString(ccel.mDataReceived).toStdString() + " instead of "
                     + dt::Utils::ToString(data + DATA_INCREMENT).toStdString() + ")" << std::endl;
        exit(1);
    }
}

int Run(int argc, char** argv) {
    QString arg1;
    if(argc > 1)
        arg1 = argv[1];
    if(arg1 != "server" && arg1 != "client") {
        std::cerr << "Usage: ./test_network < server | client >" << std::endl;
        return 1;
    }

    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    // register event prototype
    std::shared_ptr<dt::NetworkEvent> ptr(new CustomNetworkEvent(0, CustomNetworkEvent::CLIENT));
    root.GetNetworkManager()->RegisterNetworkEventPrototype(ptr);

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

} // namespace test_network

#endif
