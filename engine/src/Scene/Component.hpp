
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
class DUCTTAPE_API Component : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT FINAL)
    Q_PROPERTY(bool isEnabled READ isEnabled FINAL)
    Q_PROPERTY(bool isInitialized READ isInitialized FINAL)
    Q_PROPERTY(QScriptValue node READ getScriptNode)

public:
    /**
      * Constructor with set name.
      * @param name The Component name.
      */
    Component(const QString name = "");

    /**
      * Pure virtual destructor makes this class abstract.
      */
    virtual ~Component() = 0;

    /**
      * Called when the component is activated. Initialize all scene objects here.
      */
    virtual void onInitialize();

    /**
      * Called when the component is deactivated. Deinitialize all scene objects here.
      */
    virtual void onDeinitialize();

    /**
      * Called when the component is enabled. Show/enable all scene objects here.
      */
    virtual void onEnable();

    /**
      * Called when the component is disabled. Hide/disable all scene objects here.
      */
    virtual void onDisable();

    /**
      * Called every frame. Update the Node here.
      * @param time_diff The frame delta time.
      */
    virtual void onUpdate(double time_diff);

    /**
      * Sets the node of this component.
      * @param node The node to be set.
      */
    void setNode(Node* node);

    void serialize(IOPacket& packet);

    virtual void onSerialize(IOPacket& packet);

public slots:
    /**
      * Returns the name of the Component.
      * @returns The name of the Component.
      */
    const QString getName() const;

    /**
      * Returns the name of the Component, including all parent names.
      * @returns The name of the Component, including all parent names.
      */
    QString getFullName() const;

    /**
      * Returns the Node of this component.
      * @returns The Node of this component.
      */
    Node* getNode();

    /**
      * Returns the Node of this component. Used for scripting access.
      * @returns The Node of this component.
      */
    QScriptValue getScriptNode();

    /**
      * Returns whether the component is created.
      * @returns Whether the component is created.
      */
    bool isInitialized();

    /**
      * Returns whether the component is enabled.
      * @returns Whether the component is enabled.
      */
    bool isEnabled();

    /**
      * Initializes the component.
      */
    void initialize();

    /**
      * Deinitializes the component.
      */
    void deinitialize();

    /**
      * Enables the component.
      */
    void enable();

    /**
      * Disables the component.
      */
    void disable();

signals:
    void componentInitialized();
    void componentUninitialized();
    void componentEnabled();
    void componentDisabled();

protected:
    QString mName;  //!< The Component name.
    Node* mNode;        //!< The parent Node.

private:
    bool mIsEnabled;    //!< Whether the component is enabled or not.
    bool mIsInitialized;    //!< Whether the component has been created or not.
    QUuid mId;    //!< The id for the component.
};

} // namespace dt

#endif
