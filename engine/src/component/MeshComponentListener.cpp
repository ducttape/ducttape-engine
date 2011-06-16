#include "MeshComponentListener.hpp"

namespace dt {

void MeshComponentListener::OnAnimationStarted() {}

void MeshComponentListener::AnimationStarted() {
    _CallSignal("AnimationStarted");
    OnAnimationStarted();
}

void MeshComponentListener::OnAnimationFinished() {}

void MeshComponentListener::AnimationFinished() {
    _CallSignal("AnimationFinished");
    OnAnimationFinished();
}

}
