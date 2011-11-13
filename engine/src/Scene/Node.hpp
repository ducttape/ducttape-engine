
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_SCENE_NODE
#define DUCTTAPE_ENGINE_SCENE_NODE

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>
#include <Network/IOPacket.hpp>

#include <boost/ptr_container/ptr_map.hpp>

#include <OgreQuaternion.h>
#include <OgreVector3.h>

#include <QObject>
#include <QString>

#include <memory>

namespace dt {

// forward declaration due to circular dependency
class Scene;

/**
  * Basic scene object class.
  * Any object in a scene is described by a node with a position, scale and rotation
  * and other child nodes. The Node class is also able to have components to control
  * its behaviour, e.g. the look or events.
  * @see Component
  */
class DUCTTAPE_API Node : public QObject {
    Q_OBJECT
    Q_ENUMS(RelativeTo)

    Q_PROPERTY(QString name READ GetName CONSTANT FINAL)
    Q_PROPERTY(Node* parent READ GetParent FINAL)
    Q_PROPERTY(Scene* scene READ GetScene FINAL)

public:
    /**
      * The coordinates space for getting/setting rotation, position and scale.
      */
    enum RelativeTo {
        PARENT, //!< Relative to the parent node.
        SCENE   //!< Relative to the scene root node (i.e. absolute value).
    };

    /**
      * Constructor.
      * @param name The name of the Node.
      */
    Node(const QString& name = "");

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
      * @returns A pointer to the new component.
      */
    template <typename ComponentType>
    ComponentType* AddComponent(ComponentType* component) {
        const QString& cname = component->GetName();
        if(!HasComponent(cname)) {
            std::shared_ptr<Component> ptr(component);
            ptr->SetNode(this);
            ptr->Create();
            std::pair<QString, std::shared_ptr<Component> > pair(cname, ptr);
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
    Node* FindChildNode(const QString& name, bool recursive = true);

    /**
      * Returns a component.
      * @param name The name of the component to find.
      * @returns A pointer to the component, or nullptr if no component with the specified name exists.
      */
    template <typename ComponentType>
    ComponentType* FindComponent(const QString& name) {
        if(!HasComponent(name))
            return nullptr;
        return dynamic_cast<ComponentType*>(mComponents[name].get());
    }

    /**
      * Returns whether this node has the component assigned.
      * @param name The name of the Component.
      * @returns true if the component is assigned, otherwise false
      */
    bool HasComponent(const QString& name);

    /**
      * Removes a child Node with a specific name.
      * @param name The name of the Node to be removed.
      */
    void RemoveChildNode(const QString& name);

    /**
      * Removes a Component with a specific name.
      * @param name The name of the Component to be removed.
      */
    void RemoveComponent(const QString& name);

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
      * Sets the direction the Node is facing.
      * @param direction The direction the Node is facing.
      * @param front_vector The local direction that specifies the front of the Node (which part of the Node should be facing into the direction).
      */
    void SetDirection(Ogre::Vector3 direction, Ogre::Vector3 front_vector = Ogre::Vector3::UNIT_Z);

    /**
      * Rotates the node to look at the target.
      * @param target The point the node should look at.
      * @param front_vector The local direction that specifies the front of the Node (which part of the Node should be facing into the direction).
      * @param rel Reference position.
      */
    void LookAt(Ogre::Vector3 target, Ogre::Vector3 front_vector = Ogre::Vector3::UNIT_Z, RelativeTo rel = PARENT);

    /**
      * Sets the parent Node pointer.
      * @param parent The parent Node pointer.
      */
    void SetParent(Node* parent);

    /**
      * Called when the Node is being updated.
      * @param time_diff The frame time.
      */
    virtual void OnUpdate(double time_diff);

    void Serialize(IOPacket& packet);

    virtual void OnSerialize(IOPacket& packet);

public slots:
    /**
      * Returns the name of the Node.
      * @returns The name of the Node.
      */
    const QString& GetName() const;

    /**
      * Returns the name of the Node, including all parent names.
      * @returns The name of the Node, including all parent names.
      */
    QString GetFullName() const;

    /**
      * Returns a pointer to the parent Node.
      * @returns A pointer to the parent Node.
      */
    Node* GetParent();

    /**
      * Returns the Scene this Node is attached to.
      * @returns The Scene this Node is attached to.
      */
    Scene* GetScene();

    /**
      * Sets the position of the Node.
      * @param x The x position.
      * @param y The y position.
      * @param z The z position.
      * @param rel Reference point.
      */
    void SetPosition(float x, float y, float z, RelativeTo rel = PARENT);

signals:
    void PositionChanged();

protected:
    /**
      * Returns whether this Node is a Scene.
      * @internal
      * @returns Whether this Node is a Scene.
      */
    virtual bool _IsScene();

    /**
      * Updates all components.
      * @param time_diff The frame time.
      * @see Component::OnUpdate(double time_diff);
      */
    void _UpdateAllComponents(double time_diff);

    /**
      * Updates all child nodes.
      * @param time_diff The frame time.
      * @see OnUpdate(double time_diff);
      */
    void _UpdateAllChildren(double time_diff);

    std::map<QString, std::shared_ptr<Component> > mComponents;   //!< The list of Components.
    QString mName;              //!< The Node name.

private:
    boost::ptr_map<QString, Node> mChildren;        //!< List of child nodes.
    Ogre::Vector3 mPosition;        //!< The Node position.
    Ogre::Vector3 mScale;           //!< The Node scale.
    Ogre::Quaternion mRotation;     //!< The Node rotation.
    Node* mParent;                  //!< A pointer to the parent Node.
    bool mIsUpdatingAfterChange;    //!< Whether the node is just in the process of updating all components after a change occurred. This is to prevent infinite stack loops.
    boost::uuids::uuid mId;         //!< The node's uuid.

};

} // namespace dt

#endif
