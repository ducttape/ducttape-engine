
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "AnimableBeing.hpp"

AnimableBeing::AnimableBeing(const Ogre::Vector3& position, uint32_t hp, PlayerID player_id): 
Being(position, hp, player_id), 
mAnimationLenght(0),
mAnimationStart(0) {}

AnimableBeing::~AnimableBeing() {}


void AnimableBeing::SetAnimation(float lenght, const QString& animation_name) {
    mAnimationLenght = lenght;
    StopAnimation();
    mMeshComponent->SetLoopAnimation(true); //TODO: just set it once.
    mMeshComponent->SetAnimation(animation_name);
    mMeshComponent->PlayAnimation();
    mAnimationStart = GetNowTime();
}


void AnimableBeing::OnUpdateStart(double time_diff) {
    Being::OnUpdateStart(time_diff);
    //TODO: possible optimization here with function pointer.
    if(GetNowTime() - mAnimationStart > mAnimationLenght) {
        StopAnimation();
    }
}

void AnimableBeing::OnUpdate(double time_diff)
{
    Being::OnUpdate(time_diff);
}

void AnimableBeing::OnUpdateEnd(double time_diff)
{
    Being::OnUpdateEnd(time_diff);
}

void AnimableBeing::StopAnimation() {
    mMeshComponent->StopAnimation();
}


