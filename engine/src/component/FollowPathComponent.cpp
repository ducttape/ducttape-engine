#include "FollowPathComponent.hpp"

#include "scene/Node.hpp"

#include <boost/foreach.hpp>

namespace dt {

FollowPathComponent::FollowPathComponent(const std::string& name)
    : Component(name) {
}

void FollowPathComponent::HandleEvent(Event* e) {}

void FollowPathComponent::OnCreate() {}
void FollowPathComponent::OnDestroy() {}

void FollowPathComponent::OnUpdate(float time_diff) {
    if(IsEnabled()) {
        // move progress further
        mDurationSinceStart += time_diff;

        // calculate new position
        if(mNode != nullptr)
            mNode->SetPosition(_CalculatePosition());
    }
}

void FollowPathComponent::AddPoint(Ogre::Vector3 point) {
    mPoints.push_back(point);
    if(mPoints.size() == 1) {
        Reset();
    }
}

void FollowPathComponent::SetSpeed(float speed) {
    mTotalDuration = GetTotalLength() / speed;
}

void FollowPathComponent::SetDuration(float duration) {
    mTotalDuration = duration;
}

float FollowPathComponent::GetTotalLength() const {
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

void FollowPathComponent::Reset() {
    mDurationSinceStart = 0;
    if(mPoints.size() > 0 && mNode != nullptr) {
        mNode->SetPosition(mPoints[0]);
    }
}

Ogre::Vector3 FollowPathComponent::_CalculatePosition() {
    if(mPoints.size() == 0) {
        if(mNode != nullptr)
            return mNode->GetPosition();
        else
            return Ogre::Vector3::ZERO;
    }

    if(mPoints.size() == 1) {
        return mPoints[0];
    }

    float length_travelled = mDurationSinceStart / mTotalDuration * GetTotalLength();
    float total = 0;
    Ogre::Vector3* last = & mPoints[0];
    for(std::vector<Ogre::Vector3>::iterator iter = mPoints.begin() + 1; iter != mPoints.end(); ++iter) {
        float segment_length = (*last - *iter).length();
        if(total <= length_travelled && length_travelled <= total + segment_length) {
            // we're in the right segment
            float segment_progress = (length_travelled - total) / segment_length;
            //std::cout << *iter << std::endl;
            return (*last) + ((*iter)- (*last)) * segment_progress;
        }

        total += segment_length;
        last = &(*iter);
    }
    return mPoints.back();
}

}
