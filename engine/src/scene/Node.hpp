#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>

#include <boost/ptr_container/ptr_map.hpp>

#include "Component.hpp"

namespace dt {

class Node {
public:
    Node(const std::string& name = "");

    void AddChild(Node *child);
    void AddComponent(Component *component);

    Node* FindChildNode(const std::string& name, bool recursive = true);
    Component* FindComponent(const std::string& name);

    const std::string& GetName();

private:
    std::string mName;
    boost::ptr_map<std::string, Node*> mChildren;
    boost::ptr_map<std::string, Component*> mComponents;
};

}

#endif
