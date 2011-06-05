#ifndef DUCTTAPE_ENGINE_SCENE_NODE
#define DUCTTAPE_ENGINE_SCENE_NODE

#include <string>
#include <vector>

#include <boost/ptr_container/ptr_map.hpp>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "component/Component.hpp"

namespace dt {

// forward declaration
class Scene;

/**
  * Basic scene object class. 
  * Any object in a scene is described by a node with a position, scale and rotation
  * and other child nodes. The Node class is also able to have components to control 
  * its behaviour, e.g. the look or events.
  * @see Component
  */
class Node {
public:
    enum RelativeTo {
        PARENT,
        SCENE
    };

public:
    /**
      * Constructor.
      * @param name The name of the Node.
      */
    Node(const std::string& name = "");

    /**
      * Adds a Node as child.
      * @param child The Node to be added as child
      */
    void AddChildNode(Node *child);

    /**
      * Assigns a component to this node. The component is being added into ComponentsManager.
      * @see ComponentsManager
      * @param name The Component to be assigned.
      */
    void AddComponent(Component* component);

    /**
      * Searches for a Node with the given name and returns a pointer to the first match.
      * @param name The name of the Node searched.
      * @param recursive Whether to search within child nodes or not.
      * @returns A pointer to the Node with the name or nullptr if none is found.
      */
    Node* FindChildNode(const std::string& name, bool recursive = true);

    /**
      * Returns whether this node has the component assigned.
      * @param name The name of the Component.
      * @see ComponentsManager
      * @returns true if the component is assigned, otherwise false
      */
    bool HasComponent(const std::string& name);

    /**
      * Removes a child Node with a specific name.
      * @param name The name of the Node to be removed.
      */
    void RemoveChildNode(const std::string& name);

    /**
      * Removes a Component with a specific name.
      * @see ComponentsManager
      * @param name The name of the Component to be removed.
      */
    void RemoveComponent(const std::string& name);

    /**
      * Returns the name of the Node.
      * @returns The name of the Node.
      */
    const std::string& GetName() const;

    /**
      * Returns the position of the Node.
      * @param rel Reference point.
      * @returns The Node position.
      */
    Ogre::Vector3 GetPosition(RelativeTo rel = PARENT) const;

    /**
      * Sets the position of the Node.
      * @param position The new position of the Node.
      * @param rel Reference point.
      */
    void SetPosition(Ogre::Vector3 position, RelativeTo rel = PARENT);

    /**
      * Returns the scale of the Node.
      * @param rel Reference scale.
      * @returns The scale of the Node
      */
    Ogre::Vector3 GetScale(RelativeTo rel = PARENT) const;

    /**
      * Sets the scale of the Node.
      * @param scale The new scale.
      * @param rel Reference scale.
      */
    void SetScale(Ogre::Vector3 scale, RelativeTo rel = PARENT);

    /**
      * Sets the scale of the Node.
      * @param scale The new scale to use for all axis.
      * @param rel Reference scale.
      */
    void SetScale(Ogre::Real scale, RelativeTo rel = PARENT);

    /**
      * Returns the rotation of the Node.
      * @param rel Reference rotation.
      * @returns The Rotation of the Node.
      */
    Ogre::Quaternion GetRotation(RelativeTo rel = PARENT) const;

    /**
      * Sets the rotation of the Node.
      * @param rotation The rotation to set.
      * @param rel Reference rotation.
      */
    void SetRotation(Ogre::Quaternion rotation, RelativeTo rel = PARENT);

    /**
      * Sets the parent Node pointer.
      * @param parent The parent Node pointer.
      */
    void SetParent(Node* parent);

    /**
      * Returns a pointer to the parent Node.
      * @returns A pointer to the parent Node.
      */
    Node* GetParent();

    /**
      *
      */
    Scene* GetScene();

protected:
    virtual bool _IsScene();

private:
    std::string mName;          //!< The Node name.

    boost::ptr_map<std::string, Node> mChildren;        //!< List of child nodes.

    Ogre::Vector3 mPosition;    //!< The Node position.
    Ogre::Vector3 mScale;       //!< The Node scale.
    Ogre::Quaternion mRotation; //!< The Node rotation.

    Node* mParent;              //!< A pointer to the parent Node.
};

}

#endif
