#include "component/Component.hpp"
#include "scene/Node.hpp"

namespace dt {

Component::Component(const std::string& name) {
    if(name == "")
        mName = "component-generated-name"; // TODO
    else
        mName = name;
}

Component::~Component() {}

const std::string& Component::GetName() const {
    return mName;
}

void Component::HandleEvent(Event* e) {}

void Component::OnActivate() {}

void Component::OnDeactivate() {}

void Component::OnUpdate(float time_diff) {}

void Component::SetNode(Node* node) {
    mNode = node;
}

Node* Component::GetNode() {
    return mNode;
}

void Component::Activate() {
    mIsActivated = true;
    OnActivate();
}

void Component::Deactivate() {
    mIsActivated = false;
    OnDeactivate();
}

bool Component::IsActivated() {
    return mIsActivated;
}

boost::signals2::connection Component::BindSlot(const std::string& signal_identifier, boost::function<void ()> slot) {
    return mSignals[signal_identifier].connect(slot);
}

void Component::_CallSignal(const std::string& signal_identifier) {
    mSignals[signal_identifier]();
}

}
