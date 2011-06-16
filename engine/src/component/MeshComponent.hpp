#ifndef DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENT

#include <OGRE/OgreEntity.h>

#include "component/Component.hpp"
#include "component/MeshComponentListener.hpp"

namespace dt {

class MeshComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    MeshComponent(const std::string& name, const std::string& mesh_file = "", MeshComponentListener* custom_listener = new MeshComponentListener());

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate(float time_diff);

    void SetMeshFile(const std::string& mesh_file);
    const std::string& GetMeshFile() const;

    std::vector<std::string> GetAvailableAnimations();
    void SetAnimation(const std::string& animation_state);

    void PlayAnimation();
    void StopAnimation();
    void PauseAnimation();
    void SetLoopAnimation(bool loop_animation);
    bool GetLoopAnimation();

private:
    void _LoadMesh();
    void _DestroyMesh();

    Ogre::SceneNode* mSceneNode;
    Ogre::Entity* mEntity;
    Ogre::AnimationState* mAnimationState;
    bool mLoopAnimation;

    std::string mMeshFile;
};

}

#endif
