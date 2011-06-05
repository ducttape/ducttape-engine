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
    void OnUpdate();

    void SetMeshFile(const std::string& mesh_file);
    const std::string& GetMeshFile() const;

private:
    void _LoadMesh();
    void _DestroyMesh();

    Ogre::SceneNode* mSceneNode;
    Ogre::Entity* mEntity;

    std::string mMeshFile;
};

}

#endif
