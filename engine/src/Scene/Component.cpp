
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
      mIsCreated(false) {
    // auto-generate the component name
    if(mName == "") {
        mName = "Component-" + Utils::ToString(Utils::AutoId());
    }
}

Component::~Component() {}

const QString& Component::GetName() const {
    return mName;
}

void Component::OnCreate() {}

void Component::OnDestroy() {}

void Component::OnEnable() {}

void Component::OnDisable() {}

void Component::OnUpdate(double time_diff) {}

void Component::SetNode(Node* node) {
    mNode = node;
}

Node* Component::GetNode() {
    return mNode;
}

QScriptValue Component::GetScriptNode() {
    // Making QScriptValue from Node. Type conversion in C style only due to limitation of incomplete type.
    // return dt::ScriptManager::GetScriptEngine()->newQObject((QObject*)mNode);
    return dt::ScriptManager::Get()->GetScriptEngine()->newQObject(mNode);
}

void Component::Create() {
    if(!mIsCreated) {
        mIsCreated = true;
        mId = Utils::GenerateUUIDRandom();
        OnCreate();
        emit ComponentCreated();
        Enable();
    }
}

void Component::Destroy() {
    if(mIsCreated) {
        mIsCreated = false;
        Disable();
        emit ComponentDestroyed();
        OnDestroy();
    }
}

void Component::Enable() {
    if(!mIsEnabled) {
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

bool Component::IsCreated() {
    return mIsCreated;
}

bool Component::IsEnabled() {
    return mIsEnabled;
}

} // namespace dt
