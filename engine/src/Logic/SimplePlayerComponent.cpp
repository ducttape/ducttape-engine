// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/SimplePlayerComponent.hpp>

#include <Scene/Node.hpp>

#include <OgreMatrix3.h>

namespace dt {

SimplePlayerComponent::SimplePlayerComponent(const QString name)
    : Component(name),
      mWASDEnabled(true),
      mArrowsEnabled(true),
      mMoveSpeed(10.0),
      mMouseEnabled(true),
      mMouseSensitivity(1.0),
      mMouseYInversed(false),
      mCostant(false),
      mMove(Ogre::Vector3::ZERO) {}

void SimplePlayerComponent::onInitialize() {
    if(!QObject::connect(InputManager::Get(), SIGNAL(sPressed(dt::InputManager::InputCode input_code,
        const OIS::EventArg& event)), this, SLOT(_HandleKeyPressed(dt::InputManager::InputCode input_code, 
        const OIS::EventArg& event)))) {
            Logger::Get().Error("Cannot connect the key pressed signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
    if(mCostant) {
        if(!QObject::connect(InputManager::Get(), SIGNAL(sReleased(dt::InputManager::InputCode input_code, 
            const OIS::EventArg& event)), this, SLOT(_HandleKeyReleased(dt::InputManager::InputCode input_code, 
            const OIS::EventArg& event)))) {
                Logger::Get().Error("Cannot connect the key released signal with " + GetName()
                    + "'s keyboard input handling slot.");
        }
    }
    if(!QObject::connect(InputManager::get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)),
                         this,                SLOT(_HandleMouseInput(const OIS::MouseEvent&)))) {
            Logger::get().error("Cannot connect the mouse moved signal with " + getName()
                + "'s mouse input handling slot.");
    }
}

void SimplePlayerComponent::onDeinitialize() {}

void SimplePlayerComponent::onUpdate(double time_diff) {
        if(mMove.length() > 0) {
            static Ogre::Vector3 move;
            if(mInputStateChanged) {
               move = getNode()->getRotation() * mMove; //update movement direction
            }
            
            getNode()->setPosition(getNode()->getPosition() + move * time_diff * mMoveSpeed);
            if(!mCostant) {
                mMove = Ogre::Vector3::ZERO;
            }
        }
}

void SimplePlayerComponent::setWASDEnabled(bool wasd_enabled) {
    mWASDEnabled = wasd_enabled;
}

bool SimplePlayerComponent::getWASDEnabled() const {
    return mWASDEnabled;
}

void SimplePlayerComponent::setArrowsEnabled(bool arrows_enabled) {
    mArrowsEnabled = arrows_enabled;
}

bool SimplePlayerComponent::getArrowsEnabled() const {
    return mArrowsEnabled;
}

void SimplePlayerComponent::setMoveSpeed(float move_speed) {
    mMoveSpeed = move_speed;
}

float SimplePlayerComponent::getMoveSpeed() const {
    return mMoveSpeed;
}

void SimplePlayerComponent::setMouseEnabled(bool mouse_enabled) {
    mMouseEnabled = mouse_enabled;
}

bool SimplePlayerComponent::getMouseEnabled() const {
    return mMouseEnabled;
}

void SimplePlayerComponent::setMouseSensitivity(float mouse_sensitivity) {
    mMouseSensitivity = mouse_sensitivity;
}

float SimplePlayerComponent::getMouseSensitivity() const {
    return mMouseSensitivity;
}

void SimplePlayerComponent::setMouseYInversed(bool mouse_y_inversed) {
    mMouseYInversed = mouse_y_inversed;
}

bool SimplePlayerComponent::getMouseYInversed() const {
    return mMouseYInversed;
}

void SimplePlayerComponent::setConstant(bool constant) {
    mCostant = constant;
    if(mCostant) {
        if(!QObject::connect(InputManager::Get(), SIGNAL(sReleased(dt::InputManager::InputCode input_code, 
            const OIS::EventArg& event)), this, SLOT(_HandleKeyReleased(dt::InputManager::InputCode input_code, 
            const OIS::EventArg& event)))) {
                Logger::Get().Error("Cannot connect the key released signal with " + GetName()
                    + "'s keyboard press event slot.");
        }  
    } else {
        if(!QObject::disconnect(InputManager::Get(), SIGNAL(sReleased(dt::InputManager::InputCode input_code, 
            const OIS::EventArg& event)), this, SLOT(_HandleKeyReleased(dt::InputManager::InputCode input_code, 
            const OIS::EventArg& event)))) {
                Logger::Get().Error("Cannot disconnect the key released signal with " + GetName()
                    + "'s keyboard release event slot.");
        }
    }
}

bool SimplePlayerComponent::getConstant() const {
    return mCostant;
}

void SimplePlayerComponent::_handleMouseInput(const OIS::MouseEvent& event) {
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
        mInputStateChanged = true;
    }
}

void SimplePlayerComponent::_HandleKeyPressed(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((mWASDEnabled && input_code == InputManager::KC_W) || (mArrowsEnabled && input_code == InputManager::KC_UP)) {
            mMove.z = -1.0;
        }
        if((mWASDEnabled && input_code == InputManager::KC_S) || (mArrowsEnabled && input_code == InputManager::KC_DOWN)) {
            mMove.z = 1.0;
        }
        if((mWASDEnabled && input_code == InputManager::KC_A) || (mArrowsEnabled && input_code == InputManager::KC_LEFT)) {
            mMove.x = -1.0;
        }
        if((mWASDEnabled && input_code == InputManager::KC_D) || (mArrowsEnabled && input_code == InputManager::KC_RIGHT)) {
            mMove.x = 1.0;
        }
        mInputStateChanged = true;
    }
}

void SimplePlayerComponent::_HandleKeyReleased(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((mWASDEnabled && input_code == InputManager::KC_W) || (mArrowsEnabled && input_code == InputManager::KC_UP) || 
           (mWASDEnabled && input_code == InputManager::KC_S) || (mArrowsEnabled && input_code == InputManager::KC_DOWN)) {
            mMove.z = 0;
        }
        if((mWASDEnabled && input_code == InputManager::KC_A) || (mArrowsEnabled && input_code == InputManager::KC_LEFT) || 
           (mWASDEnabled && input_code == InputManager::KC_D) || (mArrowsEnabled && input_code == InputManager::KC_RIGHT)) {
            mMove.x = 0;
        }
        mInputStateChanged = true;
    }
}

}
