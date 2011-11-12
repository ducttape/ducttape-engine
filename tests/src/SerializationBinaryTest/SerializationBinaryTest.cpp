
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

bool SerializationBinaryTest::Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::Node node1("testnode1");
    node1.AddComponent(new dt::TriggerComponent("triggercomponent1"));
    dt::Node* child = node1.AddChildNode(new dt::Node("childnode1"));
    child->AddComponent(new dt::TriggerComponent("triggercomponent1.1"));;

    sf::Packet packet1;
    dt::IOPacket p1(&packet1, dt::IOPacket::SERIALIZE);
    node1.Serialize(p1);

    QFile file1("serialization_test1.dat");
    if(!file1.open(QIODevice::WriteOnly)) {
        dt::Logger::Get().Error("Cannot open file for writing.");
        return false;
    }
    file1.write(packet1.GetData(), packet1.GetDataSize());
    file1.close();

    dt::Logger::Get().Debug("Data length: " + dt::Utils::ToString(packet1.GetDataSize()));

    dt::IOPacket p2(&packet1, dt::IOPacket::DESERIALIZE);
    dt::Node node2("receiver");
    node2.Serialize(p2);

    dt::Logger::Get().Info("Node name: " + node2.GetName());
    if(node2.GetName() != "testnode1") {
        dt::Logger::Get().Error("The node name was not transfered.");
        return false;
    }
    if(node2.FindComponent<dt::TriggerComponent>("triggercomponent1") == nullptr) {
        dt::Logger::Get().Error("The node's component was not transfered.");
        return false;
    }
    if(node2.FindChildNode("childnode1") == nullptr) {
        dt::Logger::Get().Error("The child node was not transfered.");
        return false;
    }
    if(node2.FindChildNode("childnode1")->FindComponent<dt::TriggerComponent>("triggercomponent1.1") == nullptr) {
        dt::Logger::Get().Error("The child node's component was not transfered.");
        return false;
    }

    sf::Packet packet2;

    dt::IOPacket p3(&packet2, dt::IOPacket::SERIALIZE);
    node2.Serialize(p3);

    QFile file2("serialization_test2.dat");
    if(!file2.open(QIODevice::WriteOnly)) {
        dt::Logger::Get().Error("Cannot open file for writing.");
        return false;
    }
    file2.write(packet2.GetData(), packet2.GetDataSize());
    file2.close();

    if(!file1.open(QIODevice::ReadOnly)) {
        dt::Logger::Get().Error("Cannot open file for writing.");
        return false;
    }
    if(!file2.open(QIODevice::ReadOnly)) {
        dt::Logger::Get().Error("Cannot open file for writing.");
        return false;
    }

    if(file1.readAll() != file2.readAll()) {
        dt::Logger::Get().Error("The binary files differ.");
        return false;
    }

    file1.close();
    file2.close();
    dt::Root::GetInstance().Deinitialize();

    return true;
}

QString SerializationBinaryTest::GetTestName() {
    return "SerializationBinary";
}

}
