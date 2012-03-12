
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/AdvancedPlayerComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Input/InputManager.hpp>
#include <Logic/InteractionComponent.hpp>

#include <BtOgreGP.h>

namespace dt {

AdvancedPlayerComponent::AdvancedPlayerComponent(const QString name)
    : Component(name),
      mBtController(nullptr),
      mBtGhostObject(nullptr),
      mMouseEnabled(true),
      mMouseSensitivity(1.0),
      mMouseYInversed(false),
      mMove(0.0, 0.0, 0.0),
      mMoveSpeed(5.0f),
      mKeyboardEnabled(true),
      mJumpEnabled(true),
      mIsLeftOneShot(true),
      mIsRightOneShot(true),
      mIsMoving(false),
      mIsLeftMouseDown(false),
      mIsRightMouseDown(false) {}

void AdvancedPlayerComponent::onInitialize() {
    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition(Node::SCENE)));
    start_trans.setRotation(BtOgre::Convert::toBullet(getNode()->getRotation(Node::SCENE)));

    btScalar character_height = 1.75;
    btScalar character_width = 0.44;
    btConvexShape* capsule = new btCapsuleShape(character_width, character_height);

    mBtGhostObject = std::shared_ptr<btPairCachingGhostObject>(new btPairCachingGhostObject());
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(capsule);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //TODO: Make a better way to distinguish between AdvancedPlayerComponent and PhysicsBodyComponent.
    //For now, it's just a null pointer check to do this.
    mBtGhostObject->setUserPointer(nullptr);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    mBtController = std::shared_ptr<btKinematicCharacterController>
        (new btKinematicCharacterController(mBtGhostObject.get(), capsule, 1));
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mBtGhostObject.get());
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addAction(mBtController.get());

    if(!QObject::connect(InputManager::get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)),
                         this,                SLOT(_handleKeyDown(const OIS::KeyEvent&)))) {
            Logger::get().error("Cannot connect the key pressed signal with " + getName()
                + "'s keyboard input handling slot.");
    }
    if(!QObject::connect(InputManager::get(), SIGNAL(sKeyReleased(const OIS::KeyEvent&)),
                         this,                SLOT(_handleKeyUp(const OIS::KeyEvent&)))) {
            Logger::get().error("Cannot connect the key released signal with " + getName()
                + "'s keyboard input handling slot.");
    }
    if(!QObject::connect(InputManager::get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)),
                         this,                SLOT(_HandleMouseMove(const OIS::MouseEvent&)))) {
            Logger::get().error("Cannot connect the mouse moved signal with " + getName()
                + "'s mouse input handling slot.");
    }
    if(!QObject::connect(InputManager::get(), SIGNAL(sMousePressed(const OIS::MouseEvent&, OIS::MouseButtonID)),
                         this,                SLOT(_HandleMouseDown(const OIS::MouseEvent&, OIS::MouseButtonID)))) {
            Logger::get().error("Cannot connect the mouse moved signal with " + getName()
                + "'s mouse input handling slot.");
    }
    if(!QObject::connect(InputManager::get(), SIGNAL(sMouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID)),
                         this,                SLOT(_handleMouseUp(const OIS::MouseEvent&, OIS::MouseButtonID)))) {
            Logger::get().error("Cannot connect the mouse moved signal with " + getName()
                + "'s mouse input handling slot.");
    }
}

void AdvancedPlayerComponent::onEnable() {
    setKeyboardEnabled(true);
    setMouseEnabled(true);

    //Re-sychronize it.
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition(Node::SCENE)));
    transform.setRotation(BtOgre::Convert::toBullet(getNode()->getRotation(Node::SCENE)));

    mBtGhostObject->setWorldTransform(transform);
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mBtGhostObject.get());
}

void AdvancedPlayerComponent::onDisable() {
    setKeyboardEnabled(false);
    setMouseEnabled(false);

    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->removeCollisionObject(mBtGhostObject.get());
}

void AdvancedPlayerComponent::onUpdate(double time_diff) {
    static Ogre::Vector3 move;
    static Ogre::Quaternion quaternion;
    static btTransform trans;

    quaternion = Ogre::Quaternion(getNode()->getRotation().getYaw(), Ogre::Vector3(0.0, 1.0, 0.0));
    move = quaternion * BtOgre::Convert::toOgre(mMove) * mMoveSpeed;
    mBtController->setVelocityForTimeInterval(BtOgre::Convert::toBullet(move), 0.5);

    trans = mBtGhostObject->getWorldTransform();

    getNode()->setPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);

    if(!move.isZeroLength() && mBtController->onGround() && !mIsMoving) {
        //Emit sMove when the player starts to move when he's stopping or has done jumpping.
        emit sMove();
        mIsMoving = true;
    }
    else if((move.isZeroLength() || !mBtController->onGround()) && mIsMoving) {
        //Emit sStop when the player starts to stop or jump while he's moving.
        emit sStop();
        mIsMoving = false;
    }

    if(mIsLeftMouseDown || mIsRightMouseDown) {
        _onMouseTriggered();

        if(mIsLeftOneShot) {
            mIsLeftMouseDown = false;
        }
        if(mIsRightOneShot) {
            mIsRightMouseDown = false;
        }
    }

}

void AdvancedPlayerComponent::setKeyboardEnabled(bool is_keyboard_enabled) {
    mKeyboardEnabled = is_keyboard_enabled;

    if(!mKeyboardEnabled)
        mMove.setZero();
}

bool AdvancedPlayerComponent::getKeyboardEnabled() const {
    return mKeyboardEnabled;
}

void AdvancedPlayerComponent::setMoveSpeed(float move_speed) {
    mMoveSpeed = move_speed;
}

float AdvancedPlayerComponent::getMoveSpeed() const {
    return mMoveSpeed;
}

void AdvancedPlayerComponent::setMouseEnabled(bool mouse_enabled) {
    mMouseEnabled = mouse_enabled;

    if(!mMouseEnabled) {
        mIsLeftMouseDown = false;
        mIsRightMouseDown = false;
    }
}

bool AdvancedPlayerComponent::getMouseEnabled() const {
    return mMouseEnabled;
}

void AdvancedPlayerComponent::setMouseSensitivity(float mouse_sensitivity) {
    mMouseSensitivity = mouse_sensitivity;
}

float AdvancedPlayerComponent::getMouseSensitivity() const {
    return mMouseSensitivity;
}

void AdvancedPlayerComponent::setMouseYInversed(bool mouse_y_inversed) {
    mMouseYInversed = mouse_y_inversed;
}

bool AdvancedPlayerComponent::getMouseYInversed() const {
    return mMouseYInversed;
}

void AdvancedPlayerComponent::setJumpEnabled(bool jump_enabled) {
    mJumpEnabled = jump_enabled;
}

bool AdvancedPlayerComponent::getJumpEnabled() const{
    return mJumpEnabled;
}

void AdvancedPlayerComponent::_handleKeyDown(const OIS::KeyEvent& event) {
    if(mKeyboardEnabled) {
        if(event.key == OIS::KC_W || event.key == OIS::KC_UP) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if(event.key == OIS::KC_S || event.key == OIS::KC_DOWN) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if(event.key == OIS::KC_A || event.key == OIS::KC_LEFT) {
            mMove.setX(mMove.getX() - 1.0f);
        }
        if(event.key == OIS::KC_D || event.key == OIS::KC_RIGHT) {
            mMove.setX(mMove.getX() + 1.0f);
        }

        if(mJumpEnabled && event.key == OIS::KC_SPACE && mBtController->onGround()) {
            mBtController->jump();

            emit sStop();
            emit sJump();
        }

        _onKeyDown(event);
    }
}

void AdvancedPlayerComponent::_handleMouseMove(const OIS::MouseEvent& event) {
    if(mMouseEnabled) {
        float factor = mMouseSensitivity * -0.01;

        float dx = event.state.X.rel * factor;
        float dy = event.state.Y.rel * factor * (mMouseYInversed ? -1 : 1);

        if(dx != 0 || dy != 0) {
            // watch out for da gimbal lock !!

            Ogre::Matrix3 orientMatrix;
            getNode()->getRotation().ToRotationMatrix(orientMatrix);

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
            getNode()->setRotation(rot);
        }

        _onMouseMove(event);
    }
}

void AdvancedPlayerComponent::_handleKeyUp(const OIS::KeyEvent& event) {
    if(mKeyboardEnabled) {
        if(event.key == OIS::KC_W || event.key == OIS::KC_UP) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if(event.key == OIS::KC_S || event.key == OIS::KC_DOWN) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if(event.key == OIS::KC_A || event.key == OIS::KC_LEFT) {
            mMove.setX(mMove.getX() + 1.0f);
        }
        if(event.key == OIS::KC_D || event.key == OIS::KC_RIGHT) {
            mMove.setX(mMove.getX() - 1.0f);
        }

        _onKeyUp(event);
    }
}

void AdvancedPlayerComponent::_handleMouseDown(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    if(mMouseEnabled) {
        if(button == OIS::MB_Left) {
            mIsLeftMouseDown = true;
        }
        else if(button == OIS::MB_Right) {
            mIsRightMouseDown = true;
        }

        _onMouseDown(event, button);
    }
}

void AdvancedPlayerComponent::_handleMouseUp(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    if(mMouseEnabled) {
        if(button == OIS::MB_Left) {
            mIsLeftMouseDown = false;
        }
        else if(button == OIS::MB_Right) {
            mIsRightMouseDown = false;
        }

        _onMouseUp(event, button);
    }
}

bool AdvancedPlayerComponent::getIsOneShot(OIS::MouseButtonID mouse_button) const {
    if(mouse_button == OIS::MB_Left) {
        return mIsLeftOneShot;
    }
    else {
        return mIsRightOneShot;
    }
}

void AdvancedPlayerComponent::setIsOneShot(bool is_one_shot, OIS::MouseButtonID mouse_button) {
    if(mouse_button == OIS::MB_Left) {
        mIsLeftOneShot = is_one_shot;
    }
    else {
        mIsRightOneShot = is_one_shot;
    }
}

}
