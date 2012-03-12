// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/SimplePlayerComponent.hpp>

#include <Input/InputManager.hpp>
//#include <Input/MouseEvent.hpp>
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
    if(!QObject::connect(InputManager::get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)),
                         this,                SLOT(_HandleKeyPressed(const OIS::KeyEvent&)))) {
            Logger::get().error("Cannot connect the key pressed signal with " + getName()
                + "'s keyboard input handling slot.");
    }
    if(mCostant) {
        if(!QObject::connect(InputManager::get(), SIGNAL(sKeyReleased(const OIS::KeyEvent&)),
                             this,                SLOT(_HandleKeyReleased(const OIS::KeyEvent&)))) {
                Logger::get().error("Cannot connect the key released signal with " + getName()
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
    /*OIS::Keyboard* k = InputManager::Get()->GetKeyboard();*/

    /*if(mWASDEnabled || mArrowsEnabled) {
        Ogre::Vector3 move(Ogre::Vector3::ZERO);

        if((mWASDEnabled && k->isKeyDown(OIS::KC_W)) || (mArrowsEnabled && k->isKeyDown(OIS::KC_UP))) {
            move.z -= 1.0;
        }
        if((mWASDEnabled && k->isKeyDown(OIS::KC_S)) || (mArrowsEnabled && k->isKeyDown(OIS::KC_DOWN))) {
            move.z += 1.0;
        }
        if((mWASDEnabled && k->isKeyDown(OIS::KC_A)) || (mArrowsEnabled && k->isKeyDown(OIS::KC_LEFT))) {
            move.x -= 1.0;
        }
        if((mWASDEnabled && k->isKeyDown(OIS::KC_D)) || (mArrowsEnabled && k->isKeyDown(OIS::KC_RIGHT))) {
            move.x += 1.0;
        }*/

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
        if(!QObject::connect(InputManager::get(), SIGNAL(sKeyReleased(const OIS::KeyEvent&)),
            this,                                 SLOT(_HandleKeyReleased(const OIS::KeyEvent&)))) {
                Logger::get().error("Cannot connect the key released signal with " + getName()
                    + "'s keyboard press event slot.");
        }  
    } else {
        if(!QObject::disconnect(InputManager::get(), SIGNAL(sKeyReleased(const OIS::KeyEvent&)),
            this,                                    SLOT(_handleKeyReleased(const OIS::KeyEvent&)))) {
                Logger::get().error("Cannot disconnect the key released signal with " + getName()
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

void SimplePlayerComponent::_handleKeyPressed(const OIS::KeyEvent& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((mWASDEnabled && event.key == OIS::KC_W) || (mArrowsEnabled && event.key == OIS::KC_UP)) {
            mMove.z = -1.0;
        }
        if((mWASDEnabled && event.key == OIS::KC_S) || (mArrowsEnabled && event.key == OIS::KC_DOWN)) {
            mMove.z = 1.0;
        }
        if((mWASDEnabled && event.key == OIS::KC_A) || (mArrowsEnabled && event.key == OIS::KC_LEFT)) {
            mMove.x = -1.0;
        }
        if((mWASDEnabled && event.key == OIS::KC_D) || (mArrowsEnabled && event.key == OIS::KC_RIGHT)) {
            mMove.x = 1.0;
        }
        mInputStateChanged = true;
    }
}

void SimplePlayerComponent::_handleKeyReleased(const OIS::KeyEvent& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((mWASDEnabled && event.key == OIS::KC_W) || (mArrowsEnabled && event.key == OIS::KC_UP) || 
           (mWASDEnabled && event.key == OIS::KC_S) || (mArrowsEnabled && event.key == OIS::KC_DOWN)) {
            mMove.z = 0;
        }
        if((mWASDEnabled && event.key == OIS::KC_A) || (mArrowsEnabled && event.key == OIS::KC_LEFT) || 
           (mWASDEnabled && event.key == OIS::KC_D) || (mArrowsEnabled && event.key == OIS::KC_RIGHT)) {
            mMove.x = 0;
        }
        mInputStateChanged = true;
    }
}

}
