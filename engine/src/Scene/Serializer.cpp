
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Serializer.hpp>

namespace dt {

void Serializer::Initialize() {

}

void Serializer::Deinitialize() {

}

Component* Serializer::CreateComponent(const std::string& name) {
    int id = QMetaType::type(name.c_str());
    Logger::Get().Debug("Component type ID: " + QString::number(id));
    Component* component = static_cast<Component*>(QMetaType::construct(id));
    return component;
}

}
