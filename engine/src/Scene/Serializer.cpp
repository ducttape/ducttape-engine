
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Serializer.hpp>

#include <Logic/TriggerComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Component.hpp>

namespace dt {

void Serializer::Initialize() {
    RegisterComponent<TriggerComponent>("dt::TriggerComponent");
}

void Serializer::Deinitialize() {

}

Component* Serializer::CreateComponent(const std::string& name) {
    int id = QMetaType::type(name.c_str());
    Logger::Get().Debug("Component type ID: " + QString::number(id));
    if(id == 0) {
        Logger::Get().Error("Invalid component type: " + Utils::ToString(name) + ". Did you forget to declare it? Watch out for namespaces!");
        exit(1);
    }
    Component* component = static_cast<Component*>(QMetaType::construct(id));
    return component;
}

}
