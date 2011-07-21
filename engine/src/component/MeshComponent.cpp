
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MeshComponent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"
#include "utils/Logger.hpp"

namespace dt {

MeshComponent::MeshComponent(const std::string& name, const std::string& mesh_file)
    : Component(name) {
    mEntity = nullptr;
    mSceneNode = nullptr;
    mMeshFile = mesh_file;
    mAnimationState = nullptr;
    mPlane = nullptr;
    mIsBox = false;
    mIsSphere = false;
}

MeshComponent* MeshComponent::PlaneMesh(Ogre::Real a, Ogre::Real b, Ogre::Real c, Ogre::Real d,
                                        Ogre::Real width, Ogre::Real height, int x_segments, 
                                        int y_segments, bool normals, unsigned short num_tex_coord_sets, 
                                        Ogre::Real u_tile, Ogre::Real v_tile, const std::string& name) {

    MeshComponent* m_c = new MeshComponent(name, "");

    m_c->mPlane = new Ogre::Plane(a, b, c, d);
    m_c->mPlaneWidth = width;
    m_c->mPlaneHeight = height;
    m_c->mPlaneXSegments = x_segments;
    m_c->mPlaneYSegments = y_segments;
    m_c->mPlaneNormals = normals;
    m_c->mPlaneNumTexCoordSets = num_tex_coord_sets;
    m_c->mPlaneUTile = u_tile;
    m_c->mPlaneVTile = v_tile;

    return m_c;
    
}

MeshComponent* MeshComponent::BoxMesh(Ogre::Real width, Ogre::Real height, Ogre::Real depth, const std::string& name) {

    MeshComponent* m_c = new MeshComponent(name, "");
    m_c->mIsBox = true;
    m_c->mBoxDimenstions = Ogre::Vector3(width, height, depth);
    return m_c;

}


MeshComponent* MeshComponent::SphereMesh(Ogre::Real radius, const std::string& name)
{
    
    MeshComponent* m_c = new MeshComponent(name, "");
    m_c->mIsSphere = true;
    m_c->mSphereRadius = radius;
    return m_c;

}


void MeshComponent::HandleEvent(std::shared_ptr<Event> e) {

}

void MeshComponent::OnCreate() {

    if(mPlane != nullptr)
    {
        _LoadPlane();
    } else if (mIsBox)
    {
        _LoadBox();
    } else if (mIsSphere) {
        _LoadSphere();
    } else {
        _LoadMesh();
    }

}

void MeshComponent::OnDestroy() {
    _DestroyMesh();
}

void MeshComponent::OnUpdate(double time_diff) {
    // set position, rotation and scale of the node
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    if(mIsBox)
    {
        mSceneNode->setScale(GetNode()->GetScale(Node::SCENE)*mBoxDimenstions*0.01);
    } else if (mIsSphere) {
        mSceneNode->setScale(GetNode()->GetScale(Node::SCENE)*mSphereRadius*0.01);
    } else {
        mSceneNode->setScale(GetNode()->GetScale(Node::SCENE));
    }


    if(mAnimationState != nullptr && mAnimationState->getEnabled()) {
        mAnimationState->addTime(time_diff);
    }
}

void MeshComponent::SetMeshFile(const std::string& mesh_file) {
    if(mesh_file != mMeshFile && IsCreated()) {
        // we got a new mesh; load it
        _LoadMesh();
    }
    mMeshFile = mesh_file;
}

const std::string& MeshComponent::GetMeshFile() const {
    return mMeshFile;
}

std::vector<std::string> MeshComponent::GetAvailableAnimations() {
    std::vector<std::string> result;

    if(mEntity == nullptr)
        return result;

    Ogre::AnimationStateIterator iter = mEntity->getAllAnimationStates()->getAnimationStateIterator();
    while(iter.hasMoreElements()) {
        result.push_back(iter.current()->second->getAnimationName());
        iter.moveNext();
    }
    return result;
}

void MeshComponent::SetAnimation(const std::string& animation_state) {
    if(mEntity != nullptr) {
        mAnimationState = mEntity->getAnimationState(animation_state);
        mAnimationState->setLoop(mLoopAnimation);
    } else {
        Logger::Get().Error("Cannot set animation of component " + GetName() + ": No entity loaded.");
    }
}

void MeshComponent::PlayAnimation() {
    if(mAnimationState != nullptr) {
        mAnimationState->setEnabled(true);
    } else {
        Logger::Get().Error("Cannot play animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::StopAnimation() {
    if(mAnimationState != nullptr) {
        mAnimationState->setEnabled(false);
        mAnimationState->setTimePosition(0);
    } else {
        Logger::Get().Error("Cannot stop animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::PauseAnimation() {
    if(mAnimationState != nullptr) {
        mAnimationState->setEnabled(false);
    } else {
        Logger::Get().Error("Cannot pause animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::SetLoopAnimation(bool loop_animation) {
    mLoopAnimation = loop_animation;
    if(mAnimationState != nullptr) {
        mAnimationState->setLoop(mLoopAnimation);
    }
}

bool MeshComponent::GetLoopAnimation() {
    return mLoopAnimation;
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

void MeshComponent::_LoadPlane()
{
    const std::string& nodename = GetNode()->GetName();
    Ogre::MeshManager::getSingleton().createPlane(mName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
                                                  *mPlane, mPlaneWidth, mPlaneHeight, mPlaneXSegments, mPlaneYSegments, 
                                                  mPlaneNormals, mPlaneNumTexCoordSets, mPlaneUTile, mPlaneVTile, Ogre::Vector3::UNIT_Z);
    mEntity = GetNode()->GetScene()->GetSceneManager()->createEntity(mName + "Entity", mName);
    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode(GetNode()->GetName() + "-plane-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
}

void MeshComponent::_LoadBox()
{
    mEntity = GetNode()->GetScene()->GetSceneManager()->createEntity(mName + "Entity", Ogre::SceneManager::PT_CUBE);
    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode(GetNode()->GetName() + "-box-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
}

void MeshComponent::_LoadSphere()
{
    mEntity = GetNode()->GetScene()->GetSceneManager()->createEntity(mName + "Entity", Ogre::SceneManager::PT_SPHERE);
    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode(GetNode()->GetName() + "-sphere-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
}

}
