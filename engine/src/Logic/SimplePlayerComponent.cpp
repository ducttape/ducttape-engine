
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/SimplePlayerComponent.hpp>

#include <Input/InputManager.hpp>
#include <Input/MouseEvent.hpp>
#include <Scene/Node.hpp>

#include <OgreMatrix3.h>

namespace dt {

SimplePlayerComponent::SimplePlayerComponent(const QString& name)
    : Component(name),
      mWASDEnabled(true),
      mArrowsEnabled(true),
      mMoveSpeed(10.0),
      mMouseEnabled(true),
      mMouseSensitivity(1.0),
      mMouseYInversed(false) {}

void SimplePlayerComponent::HandleEvent(std::shared_ptr<Event> e) {
    // do not react to any events if this component is disabled
    if(!IsEnabled())
        return;

    if(mMouseEnabled && e->GetType() == "DT_MOUSEEVENT") {
        std::shared_ptr<MouseEvent> m = std::dynamic_pointer_cast<MouseEvent>(e);
        if(m->GetAction() == MouseEvent::MOVED) {
            float factor = mMouseSensitivity * -0.01;
            float dx = m->GetMouseState().X.rel * factor;
            float dy = m->GetMouseState().Y.rel * factor * (mMouseYInversed ? -1 : 1);

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
}

void SimplePlayerComponent::OnCreate() {}

void SimplePlayerComponent::OnDestroy() {}

void SimplePlayerComponent::OnUpdate(double time_diff) {
    OIS::Keyboard* k = InputManager::Get()->GetKeyboard();

    if(mWASDEnabled || mArrowsEnabled) {
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
        }

        if(move.length() > 0) {
            move.normalise(); // do not move too fast :P

            move = GetNode()->GetRotation() * move;
            GetNode()->SetPosition(GetNode()->GetPosition() + move * time_diff * mMoveSpeed);
        }
    }

}

void SimplePlayerComponent::SetWASDEnabled(bool wasd_enabled) {
    mWASDEnabled = wasd_enabled;
}

bool SimplePlayerComponent::GetWASDEnabled() const {
    return mWASDEnabled;
}

void SimplePlayerComponent::SetArrowsEnabled(bool arrows_enabled) {
    mArrowsEnabled = arrows_enabled;
}

bool SimplePlayerComponent::GetArrowsEnabled() const {
    return mArrowsEnabled;
}

void SimplePlayerComponent::SetMoveSpeed(float move_speed) {
    mMoveSpeed = move_speed;
}

float SimplePlayerComponent::GetMoveSpeed() const {
    return mMoveSpeed;
}

void SimplePlayerComponent::SetMouseEnabled(bool mouse_enabled) {
    mMouseEnabled = mouse_enabled;
}

bool SimplePlayerComponent::GetMouseEnabled() const {
    return mMouseEnabled;
}

void SimplePlayerComponent::SetMouseSensitivity(float mouse_sensitivity) {
    mMouseSensitivity = mouse_sensitivity;
}

float SimplePlayerComponent::GetMouseSensitivity() const {
    return mMouseSensitivity;
}

void SimplePlayerComponent::SetMouseYInversed(bool mouse_y_inversed) {
    mMouseYInversed = mouse_y_inversed;
}

bool SimplePlayerComponent::GetMouseYInversed() const {
    return mMouseYInversed;
}

}
