#ifndef DUCTTAPE_ENGINE_SCENE_NODE
#define DUCTTAPE_ENGINE_SCENE_NODE

#include <string>

#include <boost/ptr_container/ptr_map.hpp>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Component.hpp"

namespace dt {

/**
  * The Node class can hold other Nodes as children and is also able to use Components to control the Node.
  * @see Component
  */
class Node {
public:
    enum RelativeTo {
        PARENT,
        ROOT
    };

public:
    /**
      * Default constructor. Defaults are implementedt here.
      */
    Node();

    /**
      * Constructor
      * @param name The name of the Node.
      */
    Node(const std::string& name);

    /**
      * Adds a Node as child.
      * @param child The Node to be added to the boost::ptr_map
      */
    void AddChildNode(Node *child);

    /**
      * Adds a Component.
      * @param component The Component to be added to the boost::ptr_map
      */
    void AddComponent(Component *component);

    /**
      * FindChildNode searches for a Node with the name given and returns a pointer to the first match.
      * @param name The name of the Node searched.
      * @param recursive Search recursively default true.
      * @return A pointer to the Node searched for.
      */
    Node* FindChildNode(const std::string& name, bool recursive = true);

    /**
      * Returns a pointer to the Component with a name of name
      * @param name The name of the searched Component.
      */
    Component* FindComponent(const std::string& name);

    /**
      * Removes a child Node with a name of name.
      * @param name The name of the searched Node.
      */
    void RemoveChildNode(const std::string& name);

    /**
      * Removes a Component with a name of name.
      * @param name The name of the searched Component.
      */
    void RemoveComponent(const std::string& name);

    /**
      * Returns the name of the Node.
      * @return The name of the Node.
      */
    const std::string& GetName() const;

    /**
      * Returns the position of the Node.
      * @param rel Default is PARENT.
      * @return The Node position.
      */
    Ogre::Vector3 GetPosition(RelativeTo rel = PARENT) const;

    /**
      * Sets the position of the Node.
      * @param position The position of the Node.
      * @param rel The Relation of the Node e.g. PARENT.
      */
    void SetPosition(Ogre::Vector3 position, RelativeTo rel = PARENT);

    /**
      * Returns the scale of the Node.
      * @param rel Default is PARENT.
      * @return The scale of the Node
      */
    Ogre::Vector3 GetScale(RelativeTo rel = PARENT) const;

    /**
      * Sets the Scale of the Node.
      * @param scale The Scale Ogre::Vector to use.
      * @param rel The Relation of the Scale e.g. PARENT.
      */
    void SetScale(Ogre::Vector3 scale, RelativeTo rel = PARENT);

    /**
      * Sets the Scale of the Node.
      * @param scale The Scale Ogre::Real to use.
      * @param rel The Relation of the Scale e.g. PARENT.
      */
    void SetScale(Ogre::Real scale, RelativeTo rel = PARENT);

    /**
      * Returns the Rotation of the Node.
      * @return The Rotation of the Node.
      * @param rel The Relation of the Node e.g. PARENT.
      */
    Ogre::Quaternion GetRotation(RelativeTo rel = PARENT) const;

    /**
      * Sets the Rotation of the Node
      * @param rotation The Rotation to set.
      * @param rel The Relation of the Node e.g. PARENT.
      */
    void SetRotation(Ogre::Quaternion rotation, RelativeTo rel = PARENT);

    /**
      * Sets the parent Node pointer.
      * @param parent The parent Node pointer.
      */
    void SetParent(Node* parent);

    /**
      * Returns a pointer to the parent Node.
      * @return The pointer to the parent Node.
      */
    Node* GetParent();

private:
    /**
      * @variable The Node name.
      */
    std::string mName;

    /**
      * @variable The child Nodes of this Node.
      */
    boost::ptr_map<std::string, Node> mChildren;

    /**
      * @variable The used Components of this Node.
      */
    boost::ptr_map<std::string, Component> mComponents;

    /**
      * @variable The Node position.
      */
    Ogre::Vector3 mPosition;

    /**
      * @variable The Node scale.
      */
    Ogre::Vector3 mScale;

    /**
      * @variable The Node rotation.
      */
    Ogre::Quaternion mRotation;

    /**
      * @variable The pointer to the parent Node.
      */
    Node* mParent;
};

}

#endif
