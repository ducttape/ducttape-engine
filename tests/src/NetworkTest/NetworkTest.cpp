
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "NetworkTest/NetworkTest.hpp"

#include <iostream>

namespace NetworkTest {

bool NetworkTest::run(int argc, char** argv) {
    QString arg2;
    if(argc > 2)
        arg2 = argv[2];
    if(arg2 != "server" && arg2 != "client") {
        std::cerr << "Usage: ./test_framework Network < server | client >" << std::endl;
        return false;
    }

    dt::Root& root = dt::Root::getInstance();
    root.initialize(argc, argv);

    // register event prototype
    std::shared_ptr<dt::NetworkEvent> ptr(new CustomNetworkEvent(0, CustomNetworkEvent::CLIENT));
    root.getNetworkManager()->registerNetworkEventPrototype(ptr);

    bool result = false;
    if(arg2.toLower() == "server") {
        result = runServer();
    } else if(arg2.toLower() == "client") {
        result = runClient();
    } else {
        std::cerr << "This should not happen." << std::endl;
        result = false;
    }

    root.deinitialize();
    return result;
}

bool NetworkTest::runServer() {
    dt::Root& root = dt::Root::getInstance();
    std::cout << "-- Running server" << std::endl;

    CustomServerEventListener csel;

    dt::NetworkManager* nm = root.getNetworkManager();
    nm->bindSocket(SERVER_PORT);
    while(csel.mDataReceived == 0) {
        nm->handleIncomingEvents();
        nm->sendQueuedEvents();
        sf::sleep(sf::milliseconds(100));
        if(root.getTimeSinceInitialize() > 5.0) {
            std::cerr << "Server: Time out of 5 seconds reached" << std::endl;
            return false;
        }
    }
    return true;
}

bool NetworkTest::runClient() {
    dt::Root& root = dt::Root::getInstance();
    std::cout << "-- Running client" << std::endl;

    dt::NetworkManager* nm = root.getNetworkManager();
    nm->bindSocket(CLIENT_PORT);
    nm->connect(dt::Connection(sf::IpAddress::LocalHost, SERVER_PORT));

    int data = 1337;

    CustomClientEventListener ccel;
    root.getNetworkManager()->queueEvent(std::make_shared<CustomNetworkEvent>(data, CustomNetworkEvent::CLIENT));

    while(ccel.mDataReceived == 0) {
        nm->handleIncomingEvents();
        nm->sendQueuedEvents();
        sf::sleep(sf::milliseconds(100));
        if(root.getTimeSinceInitialize() > 5.0) {
            std::cerr << "Client: Time out of 5 seconds reached" << std::endl;
            return false;
        }
    }

    bool correct_data = ((int)ccel.mDataReceived == data + DATA_INCREMENT);
    if(!correct_data) {
        std::cerr << "Client: Received wrong data (" + dt::Utils::toStdString(dt::Utils::toString(ccel.mDataReceived)) + " instead of "
            + dt::Utils::toStdString(dt::Utils::toString(data + DATA_INCREMENT)) + ")" << std::endl;
        return false;
    }
    return true;
}

QString NetworkTest::getTestName() {
    return "Network";
}

////////////////////////////////////////////////////////////////

CustomNetworkEvent::CustomNetworkEvent(int data, Sender e)
    : mData(data),
      mEnum(e) {}

const QString CustomNetworkEvent::getType() const {
    return "CUSTOMNETWORKEVENT";
}

std::shared_ptr<dt::NetworkEvent> CustomNetworkEvent::clone() const {
    std::shared_ptr<dt::NetworkEvent> ptr(new CustomNetworkEvent(mData, mEnum));
    return ptr;
}

void CustomNetworkEvent::serialize(dt::IOPacket& p) {
    p.stream(mData, "data");
    p.stream(dt::EnumHelper(&mEnum), "enum");
}

////////////////////////////////////////////////////////////////

CustomServerEventListener::CustomServerEventListener()
    : mDataReceived(0) {
        _initialize();
}

void CustomServerEventListener::_handleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    if(e->getType() == "CUSTOMNETWORKEVENT") {
        std::shared_ptr<CustomNetworkEvent> c = std::dynamic_pointer_cast<CustomNetworkEvent>(e);
        if(c->mEnum == CustomNetworkEvent::CLIENT) {
            std::cout << "Server: received CustomNetworkEvent" << std::endl;
            // send it back, adding 1 to the data
           dt::NetworkManager::get()->
                queueEvent(std::make_shared<CustomNetworkEvent>(c->mData + DATA_INCREMENT, CustomNetworkEvent::SERVER));
            mDataReceived = c->mData;
        }
    }
}

void CustomServerEventListener::_initialize() {
    QObject::connect(dt::NetworkManager::get(), SIGNAL(newEvent(std::shared_ptr<dt::NetworkEvent>)),
                     this,                      SLOT(_handleEvent(std::shared_ptr<dt::NetworkEvent>)));
}



////////////////////////////////////////////////////////////////

CustomClientEventListener::CustomClientEventListener()
    : mDataReceived(0) {
        QObject::connect(dt::NetworkManager::get(), SIGNAL(newEvent(std::shared_ptr<dt::NetworkEvent>)),
                         this,                      SLOT(_handleEvent(std::shared_ptr<dt::NetworkEvent>)));
        _initialize();
}

void CustomClientEventListener::_handleEvent(std::shared_ptr<dt::NetworkEvent> e) {
    if(e->getType() == "CUSTOMNETWORKEVENT") {
        std::shared_ptr<CustomNetworkEvent> c = std::dynamic_pointer_cast<CustomNetworkEvent>(e);
        if(c->mEnum == CustomNetworkEvent::SERVER) {
            std::cout << "Client: received CustomNetworkEvent" << std::endl;
            mDataReceived = c->mData;
        }
    }
}

void CustomClientEventListener::_initialize() {
    QObject::connect(dt::NetworkManager::get(), SIGNAL(newEvent(std::shared_ptr<dt::NetworkEvent>)),
                     this,                      SLOT(_handleEvent(std::shared_ptr<dt::NetworkEvent>)));
}

}
