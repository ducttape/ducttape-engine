
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/AdvancePlayerComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Input/InputManager.hpp>
#include <Logic/InteractionComponent.hpp>

#include <BtOgreGP.h>

namespace dt {

AdvancePlayerComponent::AdvancePlayerComponent(const QString& name)
    : Component(name),
      mBtController(nullptr),
      mBtGhostObject(nullptr),
      mMouseEnabled(true),
      mMouseSensitivity(1.0),
      mMouseYInversed(false),
      mMove(0.0, 0.0, 0.0),
      mMoveSpeed(5.0f),
      mWASDEnabled(true),
      mArrowsEnabled(true),
      mJumpEnabled(true),
      mInteractionComponentName(""),
      mIsMouseLeftDown(false),
      mIsOneShot(true) {}

void AdvancePlayerComponent::OnCreate() {
    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE)));
    start_trans.setRotation(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)));

    btScalar character_height = 1.75;
    btScalar character_width = 0.44;
    btConvexShape* capsule = new btCapsuleShape(character_width, character_height);

    mBtGhostObject = new btPairCachingGhostObject();
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(capsule);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    mBtGhostObject->setUserPointer((void *)this);

    mBtController = new btKinematicCharacterController(mBtGhostObject, capsule, 0);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mBtGhostObject);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addAction(mBtController);

    if(!QObject::connect(InputManager::Get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)), 
        this, SLOT(_HandleKeyDown(const OIS::KeyEvent&)))) {
            Logger::Get().Error("Cannot connect the key pressed signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sKeyReleased(const OIS::KeyEvent&)), 
        this, SLOT(_HandleKeyUp(const OIS::KeyEvent&)))) {
            Logger::Get().Error("Cannot connect the key released signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)), 
        this, SLOT(_HandleMouseMove(const OIS::MouseEvent&)))) {
            Logger::Get().Error("Cannot connect the mouse moved signal with " + GetName()
                + "'s mouse input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sMousePressed(const OIS::MouseEvent&, OIS::MouseButtonID)), 
        this, SLOT(_HandleMouseDown(const OIS::MouseEvent&, OIS::MouseButtonID)))) {
            Logger::Get().Error("Cannot connect the mouse moved signal with " + GetName()
                + "'s mouse input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sMouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID)), 
        this, SLOT(_HandleMouseUp(const OIS::MouseEvent&, OIS::MouseButtonID)))) {
            Logger::Get().Error("Cannot connect the mouse moved signal with " + GetName()
                + "'s mouse input handling slot.");
    }
}

void AdvancePlayerComponent::OnDestroy() {
    if(mBtController)
        delete mBtController;
    if(mBtGhostObject)
        delete mBtGhostObject;
}

void AdvancePlayerComponent::OnEnable() {
    SetWASDEnabled(true);
    SetArrowsEnabled(true);
    SetJumpEnabled(true);
}

void AdvancePlayerComponent::OnDisable() {
    SetWASDEnabled(false);
    SetArrowsEnabled(false);
    SetJumpEnabled(false);
}

void AdvancePlayerComponent::OnUpdate(double time_diff) {
    static Ogre::Vector3 move;
    static Ogre::Quaternion quaternion;
    static btTransform trans;

    quaternion = Ogre::Quaternion(GetNode()->GetRotation().getYaw(), Ogre::Vector3(0.0, 1.0, 0.0));
    move = quaternion * BtOgre::Convert::toOgre(mMove) * mMoveSpeed;
    mBtController->setVelocityForTimeInterval(BtOgre::Convert::toBullet(move), 0.5);

    trans = mBtGhostObject->getWorldTransform();

    GetNode()->SetPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);

    if(mIsMouseLeftDown && mInteractionComponentName != "") {
        InteractionComponent* interaction_component = GetNode()->FindComponent<InteractionComponent>(mInteractionComponentName);
        if(interaction_component != nullptr) {
            interaction_component->Check();
        }

        if(interaction_component == nullptr || mIsOneShot) {
            if(interaction_component == nullptr) {
                mInteractionComponentName = "";
            }
            mIsMouseLeftDown = false;
        }
    }

}

void AdvancePlayerComponent::SetWASDEnabled(bool wasd_enabled) {
    mWASDEnabled = wasd_enabled;
}

bool AdvancePlayerComponent::GetWASDEnabled() const {
    return mWASDEnabled;
}

void AdvancePlayerComponent::SetArrowsEnabled(bool arrows_enabled) {
    mArrowsEnabled = arrows_enabled;
}

bool AdvancePlayerComponent::GetArrowsEnabled() const {
    return mArrowsEnabled;
}

void AdvancePlayerComponent::SetMoveSpeed(float move_speed) {
    mMoveSpeed = move_speed;
}

float AdvancePlayerComponent::GetMoveSpeed() const {
    return mMoveSpeed;
}

void AdvancePlayerComponent::SetMouseEnabled(bool mouse_enabled) {
    mMouseEnabled = mouse_enabled;
}

bool AdvancePlayerComponent::GetMouseEnabled() const {
    return mMouseEnabled;
}

void AdvancePlayerComponent::SetMouseSensitivity(float mouse_sensitivity) {
    mMouseSensitivity = mouse_sensitivity;
}

float AdvancePlayerComponent::GetMouseSensitivity() const {
    return mMouseSensitivity;
}

void AdvancePlayerComponent::SetMouseYInversed(bool mouse_y_inversed) {
    mMouseYInversed = mouse_y_inversed;
}

bool AdvancePlayerComponent::GetMouseYInversed() const {
    return mMouseYInversed;
}

void AdvancePlayerComponent::SetJumpEnabled(bool jump_enabled) {
    mJumpEnabled = jump_enabled;
}

bool AdvancePlayerComponent::GetJumpEnabled() const{
    return mJumpEnabled;
}

void AdvancePlayerComponent::_HandleKeyDown(const OIS::KeyEvent& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((event.key == OIS::KC_W && mWASDEnabled) || (event.key == OIS::KC_UP && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if((event.key == OIS::KC_S && mWASDEnabled) || (event.key == OIS::KC_DOWN && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if((event.key == OIS::KC_A && mWASDEnabled) || (event.key == OIS::KC_LEFT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() - 1.0f);
        }
        if((event.key == OIS::KC_D && mWASDEnabled) || (event.key == OIS::KC_RIGHT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() + 1.0f);
        }
    }
    if(mJumpEnabled && event.key == OIS::KC_SPACE) {
        mBtController->jump();
    }
}

void AdvancePlayerComponent::_HandleMouseMove(const OIS::MouseEvent& event) {
    if(mMouseEnabled) {
        float factor = mMouseSensitivity * -0.01;

        float dx = event.state.X.rel * factor;
        float dy = event.state.Y.rel * factor * (mMouseYInversed ? -1 : 1);

        if(dx != 0 || dy != 0) {
            // watch out for da gimbal lock !!

            Ogre::Matrix3 orientMatrix;
            GetNode()->GetRotation().ToRotationMatrix(orientMatrix);

            Ogre::Radian yaw, pitch, roll;
            orientMatrix.ToEulerAnglesYXZ(yaw, pitch, roll);

            pitch += Ogre::Radian(dy);
            yaw += Ogre::Radian(dx);

            // do not let it look completely vertical, or the yaw will break
            if(pitch > Ogre::Degree(89.9))
                pitch = Ogre::Degree(89.9);

            if(pitch < Ogre::Degree(-89.9))
                pitch = Ogre::Degree(-89.9);

            orientMatrix.FromEulerAnglesYXZ(yaw, pitch, roll);

            Ogre::Quaternion rot;
            rot.FromRotationMatrix(orientMatrix);
            GetNode()->SetRotation(rot);
        }
    }
}

void AdvancePlayerComponent::_HandleKeyUp(const OIS::KeyEvent& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((event.key == OIS::KC_W && mWASDEnabled) || (event.key == OIS::KC_UP && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if((event.key == OIS::KC_S && mWASDEnabled) || (event.key == OIS::KC_DOWN && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if((event.key == OIS::KC_A && mWASDEnabled) || (event.key == OIS::KC_LEFT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() + 1.0f);
        }
        if((event.key == OIS::KC_D && mWASDEnabled) || (event.key == OIS::KC_RIGHT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() - 1.0f);
        }
    }
}

void AdvancePlayerComponent::_HandleMouseDown(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    if(mMouseEnabled) {
        if(button == OIS::MB_Left) {
            mIsMouseLeftDown = true;
        }
    }
}

void AdvancePlayerComponent::_HandleMouseUp(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    if(mMouseEnabled) {
        if(button == OIS::MB_Left) {
            mIsMouseLeftDown = false;
        }
    }
}

void AdvancePlayerComponent::SetInteractionComponentName(const QString& name) {
    if(this->GetNode()->HasComponent(name) || name == "")
        mInteractionComponentName = name;
}

QString AdvancePlayerComponent::GetInteractionComponentName() const {
    return mInteractionComponentName;
}

bool AdvancePlayerComponent::GetIsOneShot() const {
    return mIsOneShot;
}

void AdvancePlayerComponent::SetIsOneShot(bool is_one_shot) {
    mIsOneShot = is_one_shot;
}

}