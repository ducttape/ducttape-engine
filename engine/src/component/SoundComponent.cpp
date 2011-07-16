#include "SoundComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

#include "utils/Logger.hpp"

namespace dt {

SoundComponent::SoundComponent(const std::string& name, 
                               const std::string& sound_file, 
                               SoundComponentListener* custom_listener)
    : Component(name, custom_listener) {
    mSoundFile = sound_file;
    _LoadSound();
}

void SoundComponent::HandleEvent(Event* e) {
    if(e->GetType() == "DT_SOUNDPAUSEEVENT") {
		if(mSound.GetStatus() == sf::Sound::Paused) {
            mSound.Play();
        } else {
            mSound.Pause();
        }
    } else if(e->GetType() == "DT_SOUNDSTOPEVENT") {
        mSound.Stop();
    } else if(e->GetType() == "DT_SOUNDSTARTEVENT") {
        mSound.Stop();
        mSound.Play();
    }
}

void SoundComponent::OnActivate() {
   _PlaySound();
}

void SoundComponent::OnDeactivate() {
    _StopSound();
}

void SoundComponent::OnUpdate(float time_diff) {
	mSound.SetPosition(mNode->GetPosition(Node::SCENE).x, 
                       mNode->GetPosition(Node::SCENE).y, 
                       mNode->GetPosition(Node::SCENE).z);
}

void SoundComponent::SetSoundFile(const std::string& sound_file) {
    if(sound_file != mSoundFile && IsActivated()) {
        // we got a new sound; load it
        _LoadSound();
    }
    mSoundFile = sound_file;
}

const std::string& SoundComponent::GetSoundFile() const {
    return mSoundFile;
}

sf::Sound& SoundComponent::GetSound() {
	return mSound;
}

void SoundComponent::_LoadSound() {
    if(mSoundFile == "") {
        Logger::Get().Error("SoundComponent [" + mName + "]: Needs a sound file.");
    }
    if(!Root::get_mutable_instance().GetResourceManager()->AddSoundBuffer(mSoundFile)) {
        Logger::Get().Error("SoundComponent [" + mName + "]: Wasn't able to load sound file [" + mSoundFile + "].");
    } else {
		mSound.SetBuffer(Root::get_mutable_instance().GetResourceManager()->GetSoundBuffer(mSoundFile));
	}
}

void SoundComponent::_PlaySound() {
    // play sound if possible
    mSound.Play();
}

void SoundComponent::_StopSound() {
    // stop sound if possible
    mSound.Stop();
}

}
