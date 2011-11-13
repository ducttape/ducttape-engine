
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENT

#include <Config.hpp>

#include <Utils/Utils.hpp>
#include <Network/IOPacket.hpp>
#include <Scene/Serializer.hpp>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <QObject>
#include <QScriptValue>
#include <QString>

#include <memory>

namespace dt {

// forward declaration due to circular dependency
class Node;

/**
  * Modifier for a node. This will add all the functionality to an otherwise empty node,
  * such as a mesh or sound.
  */
class DUCTTAPE_API Component : public QObject,
                               public boost::noncopyable {
    Q_OBJECT
    Q_PROPERTY(QString name READ GetName CONSTANT FINAL)
    Q_PROPERTY(bool isEnabled READ IsEnabled FINAL)
    Q_PROPERTY(bool isCreated READ IsCreated FINAL)
    Q_PROPERTY(QScriptValue node READ GetScriptNode)

public:
    /**
      * Constructor with set name.
      * @param name The Component name.
      */
    Component(const QString& name = "");

    /**
      * Pure virtual destructor makes this class abstract.
      */
    virtual ~Component() = 0;

    /**
      * Called when the component is activated. Create all scene objects here.
      */
    virtual void OnCreate();

    /**
      * Called when the component is deactivated. Destroy all scene objects here.
      */
    virtual void OnDestroy();

    /**
      * Called when the component is enabled. Show/enable all scene objects here.
      */
    virtual void OnEnable();

    /**
      * Called when the component is disabled. Hide/disable all scene objects here.
      */
    virtual void OnDisable();

    /**
      * Called every frame. Update the Node here.
      * @param time_diff The frame delta time.
      */
    virtual void OnUpdate(double time_diff);

    /**
      * Sets the node of this component.
      * @param node The node to be set.
      */
    void SetNode(Node* node);

    void Serialize(IOPacket& packet);

    virtual void OnSerialize(IOPacket& packet);

public slots:
    /**
      * Returns the name of the Component.
      * @returns The name of the Component.
      */
    const QString& GetName() const;

    /**
      * Returns the name of the Component, including all parent names.
      * @returns The name of the Component, including all parent names.
      */
    QString GetFullName() const;

    /**
      * Returns the Node of this component.
      * @returns The Node of this component.
      */
    Node* GetNode();

    /**
      * Returns the Node of this component. Used for scripting access.
      * @returns The Node of this component.
      */
    QScriptValue GetScriptNode();

    /**
      * Returns whether the component is created.
      * @returns Whether the component is created.
      */
    bool IsCreated();

    /**
      * Returns whether the component is enabled.
      * @returns Whether the component is enabled.
      */
    bool IsEnabled();

    /**
      * Creates the component.
      */
    void Create();

    /**
      * Destroys the component.
      */
    void Destroy();

    /**
      * Enables the component.
      */
    void Enable();

    /**
      * Disables the component.
      */
    void Disable();

signals:
    void ComponentCreated();
    void ComponentDestroyed();
    void ComponentEnabled();
    void ComponentDisabled();

protected:
    QString mName;  //!< The Component name.
    Node* mNode;        //!< The parent Node.

private:
    bool mIsEnabled;    //!< Whether the component is enabled or not.
    bool mIsCreated;    //!< Whether the component has been created or not.
    boost::uuids::uuid mId;    //!< The id for the component.
};

} // namespace dt

#endif
