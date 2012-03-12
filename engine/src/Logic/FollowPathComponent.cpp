
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/FollowPathComponent.hpp>

#include <Scene/Node.hpp>
#include <Utils/Math.hpp>

namespace dt {

FollowPathComponent::FollowPathComponent(Mode mode, const QString name)
    : Component(name),
      mDurationSinceStart(0),
      mTotalDuration(0),
      mSmoothCorners(0),
      mSmoothAcceleration(false),
      mFollowRotation(false),
      mReversed(false),
      mMode(mode) {}

void FollowPathComponent::onInitialize() {
    mLastPoint = mNode->getPosition();
}

void FollowPathComponent::onDeinitialize() {}

void FollowPathComponent::onUpdate(double time_diff) {
    // move progress further
    if(!mReversed) {
        mDurationSinceStart += time_diff;
    } else {
        mDurationSinceStart -= time_diff;
    }

    // calculate new position
    if(mNode != nullptr)
        mNode->setPosition(_calculatePosition());

    if(mFollowRotation && time_diff > 0) {
        mNode->lookAt(mLastPoint, Ogre::Vector3::NEGATIVE_UNIT_Z); // look backwards at the point we were
        mLastPoint = mNode->getPosition();
    }

    if(mDurationSinceStart > mTotalDuration || mDurationSinceStart < 0) {
        // we have travelled the whole path. now what?
        if(mMode == FollowPathComponent::LOOP) {
            reset();
        } else if(mMode == FollowPathComponent::ALTERNATING) {
            mReversed = !mReversed;
            reset();
        } else {
            // disable
            reset();
            disable();
        }
    }
}

void FollowPathComponent::addPoint(Ogre::Vector3 point) {
    mPoints.push_back(point);
    if(mPoints.size() == 1) {
        reset();
    }
}

void FollowPathComponent::addPoint(float x, float y, float z) {
    addPoint(Ogre::Vector3(x, y, z));
}

void FollowPathComponent::setSpeed(float speed) {
    mTotalDuration = getTotalLength() / speed;
}

void FollowPathComponent::setDuration(float duration) {
    mTotalDuration = duration;
}

float FollowPathComponent::getTotalLength() const {
    if(mPoints.size() <= 1)
        return 0;

    float total = 0;
    const Ogre::Vector3* last = &mPoints[0];
    for(auto iter = mPoints.begin() + 1; iter != mPoints.end(); ++iter) {
        total += (*last - *iter).length();
        last = &(*iter);
    }
    return total;
}

void FollowPathComponent::reset() {
    if(!mReversed) {
        mDurationSinceStart = 0;
        if(mPoints.size() > 0 && mNode != nullptr) {
            mNode->setPosition(mPoints.front());
        }
    } else {
        mDurationSinceStart = mTotalDuration;
        if(mPoints.size() > 0 && mNode != nullptr) {
            mNode->setPosition(mPoints.back());
        }
    }
}

Ogre::Vector3 FollowPathComponent::_calculatePosition() {
    if(mPoints.size() == 0) {
        if(mNode != nullptr)
            return mNode->getPosition();
        else
            return Ogre::Vector3::ZERO;
    }

    if(mPoints.size() == 1) {
        return mPoints[0];
    }

    float length_travelled = mDurationSinceStart / mTotalDuration * getTotalLength();
    float total = 0;
    Ogre::Vector3* last = & mPoints[0];
    for(std::vector<Ogre::Vector3>::iterator iter = mPoints.begin() + 1; iter != mPoints.end(); ++iter) {
        float segment_length = (*last - *iter).length();
        if(total <= length_travelled && length_travelled <= total + segment_length) {
            // we're in the right segment
            float segment_progress = (length_travelled - total) / segment_length;

            if(!mSmoothCorners ||
                    (iter == mPoints.begin() + 1 && segment_progress < 0.5) ||         // we're in the first part of the first segment
                    (iter == mPoints.end() - 1 && segment_progress >= 0.5)) {    // we're in the last part of the last segment
                if(mSmoothAcceleration)
                    segment_progress = Math::smoothStep(0, 1, segment_progress); // 0.5 will still be 0.5 :D

                return (*last) + ((*iter)- (*last)) * segment_progress;
            } else {
                // this is gonna be complicated ^^

                // determine the corner we are at
                std::vector<Ogre::Vector3>::iterator corner(iter - 1); // current corner is ahead
                if(segment_progress >= 0.5) {
                    corner = iter;                                  // current corner is behind
                }

                Ogre::Vector3 c(*corner);
                Ogre::Vector3 n(*(corner+1));
                Ogre::Vector3 p(*(corner-1));

                float corner_progress = segment_progress - 0.5;
                if(segment_progress < 0.5)
                    corner_progress += 1.0;

                return c + (n - c) * 0.5 * Math::bounceIn(0, 1, corner_progress, 0)
                         + (p - c) * 0.5 * Math::bounceOut(1, 0, corner_progress, 0);

                // well done!
            }
        }

        total += segment_length;
        last = &(*iter);
    }

    return mPoints.back();
}

void FollowPathComponent::setSmoothAcceleration(bool smooth_acceleration) {
    mSmoothAcceleration = smooth_acceleration;
}

bool FollowPathComponent::getSmoothAcceleration() const {
    return mSmoothAcceleration;
}

void FollowPathComponent::setSmoothCorners(bool smooth_corners) {
    mSmoothCorners = smooth_corners;
}

bool FollowPathComponent::getSmoothCorners() const {
    return mSmoothCorners;
}

void FollowPathComponent::setMode(Mode mode) {
    mMode = mode;
}

FollowPathComponent::Mode FollowPathComponent::getMode() const {
    return mMode;
}

void FollowPathComponent::setFollowRotation(bool follow_rotation) {
    mFollowRotation = follow_rotation;
}

bool FollowPathComponent::getFollowRotation() const {
    return mFollowRotation;
}

}
