#ifndef DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENTLISTENER

#include "ComponentListener.hpp"

namespace dt {

class MeshComponentListener : public ComponentListener {
public:
    virtual void OnAnimationStarted();
    void AnimationStarted();

    virtual void OnAnimationFinished();
    void AnimationFinished();

};

}

#endif
