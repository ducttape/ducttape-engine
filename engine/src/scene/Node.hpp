
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_SCENE_NODE
#define DUCTTAPE_ENGINE_SCENE_NODE

#include <string>
#include <vector>

#include <boost/ptr_container/ptr_map.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Config.hpp"
#include "utils/Logger.hpp"
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
class DUCTTAPE_API Node {
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
      * Initializer.
      */
    void Initialize();

    /**
      * Deinitializer. Cleans up the whole mess :D
      */
    void Deinitialize();

    /**
      * Called when the Node is initialized.
      */
    virtual void OnInitialize();

    /**
      * Called when the Node is deinitialized.
      */
    virtual void OnDeinitialize();

    /**
      * Adds a Node as child.
      * @param child The Node to be added as child
      * @returns A pointer to the Node.
      */
    Node* AddChildNode(Node* child);

    /**
      * Assigns a component to this node.
      * @param component The Component to be assigned.
      */
    template <typename ComponentType>
    ComponentType* AddComponent(ComponentType* component) {
        const std::string& cname = component->GetName();
        if(!HasComponent(cname)) {
            std::shared_ptr<Component> ptr(component);
            ptr->SetNode(this);
            ptr->Create();
            std::pair<std::string, std::shared_ptr<Component> > pair(cname, ptr);
            mComponents.insert(pair);

            _UpdateAllComponents(0);
        } else {
            Logger::Get().Error("Cannot add component " + cname + ": a component with this name already exists.");
        }
        return FindComponent<ComponentType>(cname);
    }

    /**
      * Searches for a Node with the given name and returns a pointer to the first match.
      * @param name The name of the Node searched.
      * @param recursive Whether to search within child nodes or not.
      * @returns A pointer to the Node with the name or nullptr if none is found.
      */
    Node* FindChildNode(const std::string& name, bool recursive = true);

    template <typename ComponentType>
    ComponentType* FindComponent(const std::string& name) {
        if(!HasComponent(name))
            return nullptr;
        return dynamic_cast<ComponentType*>(mComponents[name].get());
    }

    /**
      * Returns whether this node has the component assigned.
      * @param name The name of the Component.
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

    virtual void OnUpdate(double time_diff);

protected:
    virtual bool _IsScene();
    void _UpdateAllComponents(double time_diff);
    void _UpdateAllChildren(double time_diff);

    std::map<std::string, std::shared_ptr<Component> > mComponents;   //!< The list of Components.

    std::string mName;          //!< The Node name.

private:
    boost::ptr_map<std::string, Node> mChildren;        //!< List of child nodes.

    Ogre::Vector3 mPosition;    //!< The Node position.
    Ogre::Vector3 mScale;       //!< The Node scale.
    Ogre::Quaternion mRotation; //!< The Node rotation.

    Node* mParent;              //!< A pointer to the parent Node.

    bool mIsUpdatingAfterChange; //!< Whether the node is just in the process of updating all components after a change occurred. This is to prevent infinite stack loops.

};

}

#endif
