#include "MeshComponent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"
#include "utils/Logger.hpp"

namespace dt {

MeshComponent::MeshComponent(const std::string& name, const std::string& mesh_file, MeshComponentListener* custom_listener)
    : Component(name, custom_listener) {
    mEntity = nullptr;
    mSceneNode = nullptr;
    mMeshFile = mesh_file;
}

void MeshComponent::HandleEvent(Event* e) {

}

void MeshComponent::OnActivate() {
    _LoadMesh();
}

void MeshComponent::OnDeactivate() {
    _DestroyMesh();
}

void MeshComponent::OnUpdate() {
    // set position, rotation and scale of the node
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    mSceneNode->setScale(GetNode()->GetScale(Node::SCENE));
}

void MeshComponent::SetMeshFile(const std::string& mesh_file) {
    if(mesh_file != mMeshFile && IsActivated()) {
        // we got a new mesh; load it
        _LoadMesh();
    }
    mMeshFile = mesh_file;
}

const std::string& MeshComponent::GetMeshFile() const {
    return mMeshFile;
}

void MeshComponent::_LoadMesh() {
    // destroy existing mesh and scene node
    _DestroyMesh();

    if(mMeshFile == "") {
        Logger::Get().Error("MeshComponent ["+ mName + "]: Needs a mesh file.");
    }

    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();
    const std::string& nodename = GetNode()->GetName();
    mEntity = scene_mgr->createEntity(nodename + "-mesh-entity-" + mName, mMeshFile);
    mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(nodename + "-mesh-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
}

void MeshComponent::_DestroyMesh() {
    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();

    if(mEntity != nullptr)
        scene_mgr->destroyEntity(mEntity);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

}
