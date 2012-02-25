
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Serializer.hpp>

#include <Audio/SoundComponent.hpp>
#include <Audio/MusicComponent.hpp>

#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/ParticleSystemComponent.hpp>
#include <Graphics/TextComponent.hpp>

#include <Logic/TriggerComponent.hpp>
#include <Logic/AdvancedPlayerComponent.hpp>
#include <Logic/CollisionComponent.hpp>
#include <Logic/FollowPathComponent.hpp>
#include <Logic/InteractionComponent.hpp>
#include <Logic/RaycastComponent.hpp>
#include <Logic/ScriptComponent.hpp>
#include <Logic/SimplePlayerComponent.hpp>

#include <Physics/PhysicsBodyComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Component.hpp>

namespace dt {

void Serializer::Initialize() {
    // TODO: add OnSerialize functions to these other components
    RegisterComponent<SoundComponent>("dt::SoundComponent");
    RegisterComponent<MusicComponent>("dt::MusicComponent");
    //RegisterComponent<BillboardSetComponent>("dt::BillboardSetComponent");
    RegisterComponent<CameraComponent>("dt::CameraComponent");
    RegisterComponent<LightComponent>("dt::LightComponent");
    RegisterComponent<MeshComponent>("dt::MeshComponent");
    //RegisterComponent<ParticleSystemComponent>("dt::ParticleSystemComponent");
    //RegisterComponent<TextComponent>("dt::TextComponent");
    RegisterComponent<TriggerComponent>("dt::TriggerComponent");
    //RegisterComponent<AdvancedPlayerComponent>("dt::AdvancedPlayerComponent");
    //RegisterComponent<CollisionComponent>("dt::CollisionComponent");
    //RegisterComponent<FollowPathComponent>("dt::FollowPathComponent");
    //RegisterComponent<InteractionComponent>("dt::InteractionComponent");
    //RegisterComponent<RaycastComponent>("dt::RaycastComponent");
    //RegisterComponent<ScriptComponent>("dt::ScriptComponent");
    //RegisterComponent<SimplePlayerComponent>("dt::SimplePlayerComponent");
    //RegisterComponent<PhysicsBodyComponent>("dt::PhysicsBodyComponent");
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
