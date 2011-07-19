
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "component/PlaneComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

PlaneComponent::PlaneComponent(const std::string& name)
   : Component(name) {
}

void PlaneComponent::HandleEvent(std::shared_ptr<Event> e) {
}

void PlaneComponent::OnCreate() {
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(mName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    mEntity = GetNode()->GetScene()->GetSceneManager()->createEntity(mName + "Entity", mName);
    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode(GetNode()->GetName() + "-plane-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
    mEntity->setMaterialName("Examples/Rockwall");
}

void PlaneComponent::OnDestroy() {
    GetNode()->GetScene()->GetSceneManager()->destroyEntity(mEntity);
    GetNode()->GetScene()->GetSceneManager()->destroySceneNode(mSceneNode);
}

void PlaneComponent::OnUpdate(double time_diff) {
}

}
