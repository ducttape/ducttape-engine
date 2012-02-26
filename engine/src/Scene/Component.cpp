
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Component.hpp>

#include <Logic/ScriptManager.hpp>
#include <Scene/Node.hpp>
#include <Utils/Utils.hpp>

namespace dt {

Component::Component(const QString& name)
    : mName(name),
      mIsEnabled(false),
      mIsInitialized(false) {
    // auto-generate the component name
    if(mName == "") {
        mName = "Component-" + Utils::ToString(Utils::AutoId());
    }

    // Generate an uuid for this node.
    mId = Utils::GenerateUUIDRandom();
}

Component::~Component() {}

const QString& Component::GetName() const {
    return mName;
}

QString Component::GetFullName() const {
    return mNode->GetFullName() + "/" + GetName();
}

void Component::OnInitialize() {}

void Component::OnDeinitialize() {}

void Component::OnEnable() {}

void Component::OnDisable() {}

void Component::OnUpdate(double time_diff) {}

void Component::SetNode(Node* node) {
    mNode = node;
}

void Component::Serialize(IOPacket& packet) {
    // only write type when serializing, it will be read by the Node on deserialization
    if(packet.GetDirection() == IOPacket::SERIALIZE) {
        std::string type(metaObject()->className());
        packet.Stream(type, "type");
    }

    packet.Stream(mId, "uuid");
    packet.Stream(mName, "name");
    packet.Stream(mIsEnabled, "enabled", true);

    OnSerialize(packet);
}

void Component::OnSerialize(IOPacket& packet) {}

Node* Component::GetNode() {
    return mNode;
}

QScriptValue Component::GetScriptNode() {
    // Making QScriptValue from Node. Type conversion in C style only due to limitation of incomplete type.
    // return dt::ScriptManager::GetScriptEngine()->newQObject((QObject*)mNode);
    return dt::ScriptManager::Get()->GetScriptEngine()->newQObject(mNode);
}

void Component::Initialize() {
    if(!mIsInitialized) {
        mIsInitialized = true;
        OnInitialize();
        emit ComponentInitialized();
        Enable();
    }
}

void Component::Deinitialize() {
    if(mIsInitialized) {
        mIsInitialized = false;
        Disable();
        emit ComponentUninitialized();
        OnDeinitialize();
    }
}

void Component::Enable() {
    if(!mIsEnabled && this->GetNode()->IsEnabled()) {
        mIsEnabled = true;
        emit ComponentEnabled();
        OnEnable();
    }
}

void Component::Disable() {
    if(mIsEnabled) {
        mIsEnabled = false;
        emit ComponentDisabled();
        OnDisable();
    }
}

bool Component::IsInitialized() {
    return mIsInitialized;
}

bool Component::IsEnabled() {
    return mIsEnabled;
}

} // namespace dt
