
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "component/Component.hpp"
#include "scene/Node.hpp"

namespace dt {

Component::Component(const std::string& name) {
    if(name == "")
        mName = "component-generated-name"; // TODO
    else
        mName = name;

	mIsCreated = false;
	mIsEnabled = false;
}

Component::~Component() {}

const std::string& Component::GetName() const {
    return mName;
}

void Component::HandleEvent(Event* e) {}

void Component::OnCreate() {}

void Component::OnDestroy() {}

void Component::OnEnable() {}

void Component::OnDisable() {}

void Component::OnUpdate(float time_diff) {}

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
    }
}

void Component::Destroy() {
    if(mIsCreated) {
        mIsCreated = false;
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

boost::signals2::connection Component::BindSlot(const std::string& signal_identifier, boost::function<void ()> slot) {
    return mSignals[signal_identifier].connect(slot);
}

void Component::_CallSignal(const std::string& signal_identifier) {
    mSignals[signal_identifier]();
}

}
