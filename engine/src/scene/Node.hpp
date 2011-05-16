#ifndef DUCTTAPE_ENGINE_SCENE_NODE
#define DUCTTAPE_ENGINE_SCENE_NODE

#include <string>

#include <boost/ptr_container/ptr_map.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Component.hpp"

namespace dt {

class Node {
public:
    enum RelativeTo {
        PARENT,
        ROOT
    };

public:
    Node();
    Node(const std::string& name);

    void AddChildNode(Node *child);
    void AddComponent(Component *component);

    Node* FindChildNode(const std::string& name, bool recursive = true);
    Component* FindComponent(const std::string& name);

    void RemoveChildNode(const std::string& name);
    void RemoveComponent(const std::string& name);

    const std::string& GetName() const;

    Ogre::Vector3 GetPosition(RelativeTo rel = PARENT) const;
    void SetPosition(Ogre::Vector3 position, RelativeTo rel = PARENT);

    Ogre::Vector3 GetScale(RelativeTo rel = PARENT) const;
    void SetScale(Ogre::Vector3 scale, RelativeTo rel = PARENT);
    void SetScale(Ogre::Real scale, RelativeTo rel = PARENT);

    Ogre::Quaternion GetRotation(RelativeTo rel = PARENT) const;
    void SetRotation(Ogre::Quaternion rotation, RelativeTo rel = PARENT);

    void SetParent(Node* parent);
    Node* GetParent();

private:
    std::string mName;
    boost::ptr_map<std::string, Node> mChildren;
    boost::ptr_map<std::string, Component> mComponents;

    Ogre::Vector3 mPosition;
    Ogre::Vector3 mScale;
    Ogre::Quaternion mRotation;

    Node* mParent;
};

}

#endif
