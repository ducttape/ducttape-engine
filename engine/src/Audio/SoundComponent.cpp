
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Audio/SoundComponent.hpp>

#include <Core/ResourceManager.hpp>
#include <Scene/Node.hpp>
#include <Utils/Logger.hpp>

#include <SFML/Audio/Listener.hpp>

namespace dt {

SoundComponent::SoundComponent(const QString& sound_file_name, const QString& name)
    : Component(name),
      mSoundFileName(sound_file_name) {
    _LoadSound();
}

void SoundComponent::OnCreate() {}

void SoundComponent::OnDestroy() {}

void SoundComponent::OnUpdate(double time_diff) {
	mSound.SetPosition(mNode->GetPosition(Node::SCENE).x,
                       mNode->GetPosition(Node::SCENE).y,
                       mNode->GetPosition(Node::SCENE).z);
}

void SoundComponent::SetSoundFileName(const QString& sound_file_name) {
    if(sound_file_name != mSoundFileName && IsCreated()) {
        // we got a new sound; load it
        _LoadSound();
    }
    mSoundFileName = sound_file_name;
}

const QString& SoundComponent::GetSoundFileName() const {
    return mSoundFileName;
}

sf::Sound& SoundComponent::GetSound() {
	return mSound;
}

void SoundComponent::PlaySound() {
    if(mSound.GetStatus() != sf::Sound::Playing) {
        // play sound if possible and enabled
        if(IsEnabled())
            mSound.Play();
        emit SoundPlayed();
    }
}

void SoundComponent::PauseSound() {
    if(mSound.GetStatus() != sf::Sound::Paused) {
        // pause sound if possible
        mSound.Pause();
        emit SoundPaused();
    }
}

void SoundComponent::StopSound() {
    if(mSound.GetStatus() != sf::Sound::Stopped) {
        // stop sound if possible
        mSound.Stop();
        mSound.SetPlayingOffset(0); // rewind
        emit SoundStopped();
    }
}

void SoundComponent::_LoadSound() {
    if(mSoundFileName == "") {
        Logger::Get().Error("SoundComponent [" + mName + "]: Needs a sound file.");
    }
    if(!ResourceManager::Get()->AddSoundBuffer(mSoundFileName)) {
        Logger::Get().Error("SoundComponent [" + mName + "]: Wasn't able to load sound file [" + mSoundFileName + "].");
    } else {
        mSound.SetBuffer(*ResourceManager::Get()->GetSoundBuffer(mSoundFileName));
    }
}

void SoundComponent::SetVolume(float volume) {
    if(mSound.GetVolume() != volume) {
        mSound.SetVolume(volume);
        emit VolumeChanged(volume);
    }
}

void SoundComponent::SetMasterVolume(float volume) {
    if(sf::Listener::GetGlobalVolume() != volume) {
        sf::Listener::SetGlobalVolume(volume);
        emit MasterVolumeChanged(volume);
    }
}

}
