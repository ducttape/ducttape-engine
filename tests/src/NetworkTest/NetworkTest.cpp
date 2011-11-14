
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "NetworkTest/NetworkTest.hpp"

#include <iostream>

namespace NetworkTest {

bool NetworkTest::Run(int argc, char** argv) {
    QString arg2;
    if(argc > 2)
        arg2 = argv[2];
    if(arg2 != "server" && arg2 != "client") {
        std::cerr << "Usage: ./test_framework Network < server | client >" << std::endl;
        return false;
    }

    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    // register event prototype
    std::shared_ptr<dt::NetworkEvent> ptr(new CustomNetworkEvent(0, CustomNetworkEvent::CLIENT));
    root.GetNetworkManager()->RegisterNetworkEventPrototype(ptr);

    bool result = false;
    if(arg2.toLower() == "server") {
        result = RunServer();
    } else if(arg2.toLower() == "client") {
        result = RunClient();
    } else {
        std::cerr << "This should not happen." << std::endl;
        result = false;
    }

    root.Deinitialize();
    return result;
}

bool NetworkTest::RunServer() {
    dt::Root& root = dt::Root::GetInstance();
    std::cout << "-- Running server" << std::endl;

    CustomServerEventListener csel;

    dt::NetworkManager* nm = root.GetNetworkManager();
    nm->BindSocket(SERVER_PORT);
    while(csel.mDataReceived == 0) {
        nm->HandleIncomingEvents();
        nm->SendQueuedEvents();
        sf::Sleep(100);
        if(root.GetTimeSinceInitialize() > 5.0) {
            std::cerr << "Server: Time out of 5 seconds reached" << std::endl;
            return false;
        }
    }
    return true;
}

bool NetworkTest::RunClient() {
    dt::Root& root = dt::Root::GetInstance();
    std::cout << "-- Running client" << std::endl;

    dt::NetworkManager* nm = root.GetNetworkManager();
    nm->BindSocket(CLIENT_PORT);
    nm->Connect(dt::Connection(sf::IpAddress::LocalHost, SERVER_PORT));

    int data = 1337;

    CustomClientEventListener ccel;
    root.GetNetworkManager()->QueueEvent(std::make_shared<CustomNetworkEvent>(data, CustomNetworkEvent::CLIENT));

    while(ccel.mDataReceived == 0) {
        nm->HandleIncomingEvents();
        nm->SendQueuedEvents();
        sf::Sleep(100);
        if(root.GetTimeSinceInitialize() > 5.0) {
            std::cerr << "Client: Time out of 5 seconds reached" << std::endl;
            return false;
        }
    }

    bool correct_data = ((int)ccel.mDataReceived == data + DATA_INCREMENT);
    if(!correct_data) {
        std::cerr << "Client: Received wrong data (" + dt::Utils::ToStdString(dt::Utils::ToString(ccel.mDataReceived)) + " instead of "
            + dt::Utils::ToStdString(dt::Utils::ToString(data + DATA_INCREMENT)) + ")" << std::endl;
        return false;
    }
    return true;
}

QString NetworkTest::GetTestName() {
    return "Network";
}

////////////////////////////////////////////////////////////////

CustomNetworkEvent::CustomNetworkEvent(int data, Sender e)
    : mData(data),
      mEnum(e) {}

const QString CustomNetworkEvent::GetType() const {
    return "CUSTOMNETWORKEVENT";
}

std::shared_ptr<dt::NetworkEvent> CustomNetworkEvent::Clone() const {
    std::shared_ptr<dt::NetworkEvent> ptr(new CustomNetworkEvent(mData, mEnum));
    return ptr;
}

void CustomNetworkEvent::Serialize(dt::IOPacket& p) {
    p.Stream(mData, "data");
    p.Stream(dt::EnumHelper(&mEnum), "enum");
}

////////////////////////////////////////////////////////////////

CustomServerEventListener::CustomServerEventListener()
    : mDataReceived(0) {
        _Initialize();
}

void CustomServerEventListener::_HandleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    if(e->GetType() == "CUSTOMNETWORKEVENT") {
        std::shared_ptr<CustomNetworkEvent> c = std::dynamic_pointer_cast<CustomNetworkEvent>(e);
        if(c->mEnum == CustomNetworkEvent::CLIENT) {
            std::cout << "Server: received CustomNetworkEvent" << std::endl;
            // send it back, adding 1 to the data
           dt::NetworkManager::Get()->
                QueueEvent(std::make_shared<CustomNetworkEvent>(c->mData + DATA_INCREMENT, CustomNetworkEvent::SERVER));
            mDataReceived = c->mData;
        }
    }
}

void CustomServerEventListener::_Initialize() {
    QObject::connect(dt::NetworkManager::Get(), SIGNAL(NewEvent(std::shared_ptr<dt::NetworkEvent>)),
        this, SLOT(_HandleEvent(std::shared_ptr<dt::NetworkEvent>)));
}



////////////////////////////////////////////////////////////////

CustomClientEventListener::CustomClientEventListener()
    : mDataReceived(0) {
        QObject::connect(dt::NetworkManager::Get(), SIGNAL(NewEvent(std::shared_ptr<dt::NetworkEvent>)),
            this, SLOT(_HandleEvent(std::shared_ptr<dt::NetworkEvent>)));
        _Initialize();
}

void CustomClientEventListener::_HandleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    if(e->GetType() == "CUSTOMNETWORKEVENT") {
        std::shared_ptr<CustomNetworkEvent> c = std::dynamic_pointer_cast<CustomNetworkEvent>(e);
        if(c->mEnum == CustomNetworkEvent::SERVER) {
            std::cout << "Client: received CustomNetworkEvent" << std::endl;
            mDataReceived = c->mData;
        }
    }
}

void CustomClientEventListener::_Initialize() {
    QObject::connect(dt::NetworkManager::Get(), SIGNAL(NewEvent(std::shared_ptr<dt::NetworkEvent>)),
        this, SLOT(_HandleEvent(std::shared_ptr<dt::NetworkEvent>)));
}

}
