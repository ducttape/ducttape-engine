
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Component.hpp>

#include <Core/StringManager.hpp>
#include <Event/EventManager.hpp>
#include <Scene/Node.hpp>
#include <Utils/Utils.hpp>

namespace dt {

Component::Component(const QString& name)
    : mName(name),
      mIsEnabled(false),
      mIsCreated(false) {
    // auto-generate the component name
    if(mName == "") {
        mName = "Component-" + Utils::ToString(StringManager::Get()->GetNextAutoId());
    }
}

Component::~Component() {}

const QString& Component::GetName() const {
    return mName;
}

void Component::HandleEvent(std::shared_ptr<Event> e) {}

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

void Component::Create() {
    if(!mIsCreated) {
        mIsCreated = true;
        OnCreate();
        Enable();
    }
    EventManager::Get()->AddListener(this);
}

void Component::Destroy() {
    EventManager::Get()->RemoveListener(this);
    if(mIsCreated) {
        mIsCreated = false;
        Disable();
        OnDestroy();
    }
}

void Component::Enable() {
    if(!mIsEnabled) {
        mIsEnabled = true;
        OnEnable();
    }
}

void Component::Disable() {
    if(mIsEnabled) {
        mIsEnabled = false;
        OnDisable();
    }
}

bool Component::IsCreated() {
    return mIsCreated;
}

bool Component::IsEnabled() {
    return mIsEnabled;
}

boost::signals2::connection Component::BindSlot(const QString& signal_identifier, boost::function<void ()> slot) {
    return mSignals[signal_identifier].connect(slot);
}

void Component::_CallSignal(const QString& signal_identifier) {
    mSignals[signal_identifier]();
}

} // namespace dt
