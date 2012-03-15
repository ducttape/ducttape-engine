
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SerializationYamlTest/SerializationYamlTest.hpp"

#include <Scene/Serializer.hpp>
#include <Scene/Node.hpp>
#include <Logic/TriggerComponent.hpp>

#include <QFile>
#include <QTextStream>

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>

namespace SerializationYamlTest {

bool SerializationYamlTest::run(int argc, char** argv) {
    dt::Root::getInstance().initialize(argc, argv);

    dt::Node node1("testnode1");
    node1.addComponent(new dt::TriggerComponent("triggercomponent1"));
    std::shared_ptr<dt::Node> child = node1.addChildNode(new dt::Node("childnode1"));
    child->addComponent(new dt::TriggerComponent("triggercomponent1.1"));;

    YAML::Emitter emit1;
    dt::IOPacket p1(&emit1);
    emit1 << YAML::BeginMap;
    node1.serialize(p1);
    emit1 << YAML::EndMap;

    QFile file1("serialization_test1.yaml");
    if(!file1.open(QIODevice::WriteOnly)) {
        dt::Logger::get().error("Cannot open file for writing.");
        return false;
    }
    QTextStream out(&file1);
    out << QString(emit1.c_str());
    file1.close();

    std::ifstream fin("serialization_test1.yaml");
    YAML::Parser parser(fin);
    YAML::Node doc;
    if(!parser.GetNextDocument(doc)) {
        dt::Logger::get().error("Cannot read YAML document.");
        return false;
    }

    dt::IOPacket p2(&doc);
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

    YAML::Emitter emit2;
    emit2 << YAML::BeginMap;
    dt::IOPacket p3(&emit2);
    node2.serialize(p3);
    emit2 << YAML::EndMap;

    QFile file2("serialization_test2.yaml");
    if(!file2.open(QIODevice::WriteOnly)) {
        dt::Logger::get().error("Cannot open file for writing.");
        return false;
    }
    file2.write(emit2.c_str());
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
        dt::Logger::get().error("The YAML files differ.");
        return false;
    }

    file1.close();
    file2.close();
    dt::Root::getInstance().deinitialize();

    return true;
}

QString SerializationYamlTest::getTestName() {
    return "SerializationYaml";
}

}
