
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_MESHCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <OgreAnimationState.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include <QString>

#include <memory>
#include <vector>

namespace dt {

/**
  * Component to add a mesh to a node. Animations are supported.
  */
class DUCTTAPE_API MeshComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @param material_name The name of the material to assign to the mesh.
      * Empty string to keep the mesh's material name. Default: none.
      * @param mesh_handle The handle the mesh is loaded from. This could be
      * a file path or a generated's mesh name.
      * @see Component
      */
    MeshComponent(const QString& mesh_handle = "",
                  const QString& material_name = "", const QString& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
      * Sets the handle the mesh is being loaded from.
      * @param mesh_handle The handle of the mesh.
      */
    void SetMeshHandle(const QString& mesh_handle);

    /**
      * Returns the name of the mesh handle.
      * @returns The name of the mesh handle. This could be a file path or a
      * generated's mesh name.
      */
    const QString& GetMeshHandle() const;

    /**
      * Returns a list of available animations for the loaded mesh.
      * @returns A list of available animations for the loaded mesh.
      */
    std::vector<QString> GetAvailableAnimations();

    /**
      * Sets the animation to use.
      * @param animation_state The identifier (name) of the Ogre::AnimationState to use.
      * @see Ogre::AnimationState
      */
    void SetAnimation(const QString& animation_state);

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

    /**
      * Sets the material name of the mesh.
      * @param material_name The new Ogre material name.
      */
    void SetMaterialName(const QString& material_name);

    /**
      * Gets the Ogre::SceneNode.
      * @returns The Ogre::SceneNode this mesh is attached to.
      */
    Ogre::SceneNode* GetOgreSceneNode() const;

    /**
      * Gets the Ogre::Entity;
      * @returns The Ogre::Entity representing this mesh.
      */
    Ogre::Entity* GetOgreEntity() const;

    /**
      * Sets whether the mesh should cast shadows. Default: true.
      * @param shadow Whether the mesh should cast shadows.
      */
    void SetCastShadows(bool cast_shadows);

    /**
      * Gets whether the mesh should cast shadows.
      * @returns Whether the mesh should cast shadows.
      */
    bool GetCastShadows() const;
private:
    /**
      * Private method. Loads the mesh handle.
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

    QString mMeshHandle;        //!< The handle of the mesh.
    QString mMaterialName;      //!< The name of the material to apply to the mesh.
    bool mCastShadows;              //!< Whether the mesh should cast shadows.
};

}

#endif
