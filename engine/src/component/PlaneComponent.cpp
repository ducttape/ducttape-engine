#include "component/PlaneComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

PlaneComponent::PlaneComponent(const std::string& name)
   : Component(name) {
}

void PlaneComponent::HandleEvent(Event* e) {
}

void PlaneComponent::OnActivate() {
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(mName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    mEntity = GetNode()->GetScene()->GetSceneManager()->createEntity(mName + "Entity", mName);
    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode(GetNode()->GetName() + "-plane-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
    mEntity->setMaterialName("Examples/Rockwall");
}

void PlaneComponent::OnDeactivate() {
    GetNode()->GetScene()->GetSceneManager()->destroyEntity(mEntity);
    GetNode()->GetScene()->GetSceneManager()->destroySceneNode(mSceneNode);
}

void PlaneComponent::OnUpdate(float time_diff) {
}

}
