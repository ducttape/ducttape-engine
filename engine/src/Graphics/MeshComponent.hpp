
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
    DT_SERIALIZABLE(MeshComponent)
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @param material_name The name of the material to assign to the mesh.
      * Empty string to keep the mesh's material name. Default: none.
      * @param mesh_handle The handle the mesh is loaded from. This could be
      * a file path or a generated's mesh name.
      * @see Component
      */
    MeshComponent(const QString mesh_handle = "",
                  const QString material_name = "", const QString name = "");

    void onInitialize();
    void onDeinitialize();
    void onEnable();
    void onDisable();
    void onUpdate(double time_diff);
    void onSerialize(IOPacket &packet);

    /**
      * Sets the handle the mesh is being loaded from.
      * @param mesh_handle The handle of the mesh.
      */
    void setMeshHandle(const QString mesh_handle);

    /**
      * Returns the name of the mesh handle.
      * @returns The name of the mesh handle. This could be a file path or a
      * generated's mesh name.
      */
    const QString getMeshHandle() const;

    /**
      * Returns a list of available animations for the loaded mesh.
      * @returns A list of available animations for the loaded mesh.
      */
    std::vector<QString> getAvailableAnimations();

    /**
      * Sets the animation to use.
      * @param animation_state The identifier (name) of the Ogre::AnimationState to use.
      * @see Ogre::AnimationState
      */
    void setAnimation(const QString animation_state);

    /**
      * Sets whether the animation should be looped or not.
      * @param loop_animation Whether the animation should be looped or not.
      */
    void setLoopAnimation(bool loop_animation);

    /**
      * Returns whether the animation is being looped or not.
      * @returns Whether the animation is being looped or not.
      */
    bool getLoopAnimation();

    /**
      * Sets the material name of the mesh.
      * @param material_name The new Ogre material name.
      */
    void setMaterialName(const QString material_name);

    /**
      * Gets the Ogre::SceneNode.
      * @returns The Ogre::SceneNode this mesh is attached to.
      */
    Ogre::SceneNode* getOgreSceneNode() const;

    /**
      * Gets the Ogre::Entity;
      * @returns The Ogre::Entity representing this mesh.
      */
    Ogre::Entity* getOgreEntity() const;

    /**
      * Sets whether the mesh should cast shadows. Default: true.
      * @param shadow Whether the mesh should cast shadows.
      */
    void setCastShadows(bool cast_shadows);

    /**
      * Gets whether the mesh should cast shadows.
      * @returns Whether the mesh should cast shadows.
      */
    bool getCastShadows() const;

public slots:
    /**
      * Plays the current animation.
      */
    void playAnimation();

    /**
      * Stops the current animation.
      */
    void stopAnimation();

    /**
      * Pauses the current animation.
      */
    void pauseAnimation();

signals:
    void animationPlayed();
    void animationStopped();
    void animationPaused();

private:
    /**
      * Private method. Loads the mesh handle.
      */
    void _loadMesh();

    /**
      * Private method. Destroys the mesh.
      */
    void _destroyMesh();

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
