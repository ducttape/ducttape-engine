
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_SCENE_SERIALIZER
#define DUCTTAPE_ENGINE_SCENE_SERIALIZER

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

#include <QMetaType>
/**
  * Macro definition for simple creation of "fake copy-constructor".
  * @param class_name The name of the current class
  */
#define DT_SERIALIZABLE(class_name) DT_SERIALIZABLE_INHERITED(class_name, Component)

#define DT_SERIALIZABLE_INHERITED(class_name, base_class_name) \
    class_name(const class_name& other) : base_class_name() { \
        assert(false && "This copy-constructor should never be called. (class_name)"); \
    }

namespace dt {

// forward declaration
class Component;
class Node;

/**
  * Basic scene object class.
  * Any object in a scene is described by a node with a position, scale and rotation
  * and other child nodes. The Node class is also able to have components to control
  * its behaviour, e.g. the look or events.
  * @see Component
  */
class DUCTTAPE_API Serializer {
public:
    /**
      * Initializer.
      */
    static void Initialize();

    /**
      * Deinitializer. Cleans up the whole mess :D
      */
    static void Deinitialize();

    template <typename T>
    static void RegisterComponent(const std::string& name) {
        qRegisterMetaType<T>(name.c_str());
    }

    static Component* CreateComponent(const std::string& name);

    static void SerializeNode(Node* node);

private:


};

} // namespace dt

#endif
