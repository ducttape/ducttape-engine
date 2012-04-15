
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SerializationBinaryTest/SerializationBinaryTest.hpp"

#include <Scene/Serializer.hpp>
#include <Scene/Node.hpp>
#include <Logic/TriggerComponent.hpp>

#include <iostream>

#include <QFile>

namespace SerializationBinaryTest {

bool SerializationBinaryTest::run(int argc, char** argv) {
    dt::Root::getInstance().initialize(argc, argv);

    dt::Node node1("testnode1");
    node1.addComponent(new dt::TriggerComponent("triggercomponent1"));
    auto child = node1.addChildNode(new dt::Node("childnode1"));
    child->addComponent(new dt::TriggerComponent("triggercomponent1.1"));;

    sf::Packet packet1;
    dt::IOPacket p1(&packet1, dt::IOPacket::SERIALIZE);
    node1.serialize(p1);

    QFile file1("serialization_test1.dat");
    if(!file1.open(QIODevice::WriteOnly)) {
        dt::Logger::get().error("Cannot open file for writing.");
        return false;
    }
    file1.write((const char*)packet1.getData(), packet1.getDataSize());
    file1.close();

    dt::Logger::get().debug("Data length: " + dt::Utils::toString(packet1.getDataSize()));

    dt::IOPacket p2(&packet1, dt::IOPacket::DESERIALIZE);
    dt::Node node2("receiver");
    node2.serialize(p2);

    dt::Logger::get().info("Node name: " + node2.getName());
    if(node2.getName() != "testnode1") {
        dt::Logger::get().error("The node name was not transfered.");
        return false;
    }
    if(node2.findComponent<dt::TriggerComponent>("triggercomponent1") == nullptr) {
        dt::Logger::get().error("The node's component was not transfered.");
        return false;
    }
    if(node2.findChildNode("childnode1") == nullptr) {
        dt::Logger::get().error("The child node was not transfered.");
        return false;
    }
    if(node2.findChildNode("childnode1")->findComponent<dt::TriggerComponent>("triggercomponent1.1") == nullptr) {
        dt::Logger::get().error("The child node's component was not transfered.");
        return false;
    }

    sf::Packet packet2;

    dt::IOPacket p3(&packet2, dt::IOPacket::SERIALIZE);
    node2.serialize(p3);

    QFile file2("serialization_test2.dat");
    if(!file2.open(QIODevice::WriteOnly)) {
        dt::Logger::get().error("Cannot open file for writing.");
        return false;
    }
    file2.write((const char*)packet2.getData(), packet2.getDataSize());
    file2.close();

    if(!file1.open(QIODevice::ReadOnly)) {
        dt::Logger::get().error("Cannot open file for writing.");
        return false;
    }
    if(!file2.open(QIODevice::ReadOnly)) {
        dt::Logger::get().error("Cannot open file for writing.");
        return false;
    }

    if(file1.readAll() != file2.readAll()) {
        dt::Logger::get().error("The binary files differ.");
        return false;
    }

    file1.close();
    file2.close();
    dt::Root::getInstance().deinitialize();

    return true;
}

QString SerializationBinaryTest::getTestName() {
    return "SerializationBinary";
}

}
