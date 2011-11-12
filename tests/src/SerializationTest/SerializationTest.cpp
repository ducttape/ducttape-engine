
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SerializationTest/SerializationTest.hpp"

#include <Scene/Serializer.hpp>
#include <Scene/Node.hpp>
#include <Logic/TriggerComponent.hpp>

#include <iostream>

#include <QFile>

namespace SerializationTest {

bool SerializationTest::Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::Node node("testnode1");
    node.AddComponent(new dt::TriggerComponent("triggercomponent1"));
    dt::Node* child = node.AddChildNode(new dt::Node("childnode1"));
    child->AddComponent(new dt::TriggerComponent("triggercomponent1.1"));;

    sf::Packet packet;
    dt::IOPacket p(&packet, dt::IOPacket::MODE_SEND);
    node.Serialize(p);

    QFile file("serialization_test.dat");
    if(!file.open(QIODevice::WriteOnly)) {
        dt::Logger::Get().Error("Cannot open file for writing.");
        return false;
    }
    file.write(packet.GetData(), packet.GetDataSize());
    file.close();

    dt::Logger::Get().Debug("Data length: " + dt::Utils::ToString(packet.GetDataSize()));

    dt::Root::GetInstance().Deinitialize();
    return true;
}

QString SerializationTest::GetTestName() {
    return "Serialization";
}

}
