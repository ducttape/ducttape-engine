
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENT

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Event/EventListener.hpp>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/signals2.hpp>

#include <QObject>
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
                               public EventListener,
                               public boost::noncopyable {
    Q_OBJECT
    Q_PROPERTY(bool mIsEnabled READ IsEnabled)
    Q_PROPERTY(QString mName READ GetName CONSTANT)

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
      * Returns the name of the Component.
      * @returns The name of the Component.
      */
    const QString& GetName() const;

    virtual void HandleEvent(std::shared_ptr<Event> e);

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

    /**
      * Returns the Node of this component.
      * @returns The Node of this component.
      */
    Node* GetNode();

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
      * Binds a slot to a signal.
      * @param signal_identifier The name of the signal being called on events. Example: "Triggered" or "AnimationFinished". Case sensitive.
      * @param slot The callback function.
      * @returns A boost signals2 connection to disconnect the slot again.
      */
    boost::signals2::connection BindSlot(const QString& signal_identifier, boost::function<void ()> slot);

protected:

    /**
      * Calls the signal with the identifier given.
      * @param signal_identifier The signal to call.
      */
    void _CallSignal(const QString& signal_identifier);
    boost::ptr_map<QString, boost::signals2::signal<void ()> > mSignals;    //!< The list of signals.

    QString mName;  //!< The Component name.
    Node* mNode;        //!< The parent Node.

private:
    bool mIsEnabled;    //!< Whether the component is enabled or not.
    bool mIsCreated;    //!< Whether the component has been created or not.
};

} // namespace dt

#endif
