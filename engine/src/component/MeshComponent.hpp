
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENT

#include <OGRE/OgreEntity.h>

#include "component/Component.hpp"

namespace dt {

/**
  * Component to add a mesh to a node. Animations are supported.
  */
class MeshComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @param mesh_file The file the mesh is loaded from.
      * @see Component
      */
    MeshComponent(const std::string& name, const std::string& mesh_file);   // TODO: Figure out way to have default name here

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

    /**
      * Sets the file the mesh is being loaded from.
      * @param mesh_file The filename of the mesh.
      */
    void SetMeshFile(const std::string& mesh_file);

    /**
      * Returns the name of the mesh file.
      * @returns The name of the mesh file.
      */
    const std::string& GetMeshFile() const;

    /**
      * Returns a list of available animations for the loaded mesh.
      * @returns A list of available animations for the loaded mesh.
      */
    std::vector<std::string> GetAvailableAnimations();

    /**
      * Sets the animation to use.
      * @param animation_state The identifier (name) of the Ogre::AnimationState to use.
      * @see Ogre::AnimationState
      */
    void SetAnimation(const std::string& animation_state);

    /**
      * Plays the current animation.
      */
    void PlayAnimation();

    /**
      * Stops the current animation.
      */
    void StopAnimation();

    /**
      * Pauses the current animation.
      */
    void PauseAnimation();

    /**
      * Sets whether the animation should be looped or not.
      * @param loop_animation Whether the animation should be looped or not.
      */
    void SetLoopAnimation(bool loop_animation);

    /**
      * Returns whether the animation is being looped or not.
      * @returns Whether the animation is being looped or not.
      */
    bool GetLoopAnimation();

private:
    /**
      * Private method. Loads the mesh file.
      */
    void _LoadMesh();

    /**
      * Private method. Destroys the mesh.
      */
    void _DestroyMesh();

    Ogre::SceneNode* mSceneNode;    //!< The scene Node the mesh is being attached to.
    Ogre::Entity* mEntity;          //!< The actual mesh.
    Ogre::AnimationState* mAnimationState;  //!< The current animation state.
    bool mLoopAnimation;            //!< Whether the animation shall be looped.

    std::string mMeshFile;          //!< The filename of the mesh file.
};

}

#endif
