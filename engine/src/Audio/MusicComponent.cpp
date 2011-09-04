
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Audio/MusicComponent.hpp>

#include <Core/ResourceManager.hpp>
#include <Utils/Logger.hpp>

#include <SFML/Audio/Listener.hpp>

namespace dt {

MusicComponent::MusicComponent(const QString& music_file_name, const QString& name)
    : Component(name),
      mMusicFileName(music_file_name),
      mFadeFlag(false),
      mElapsedTime(0.0),
      mFadeTime(0.0),
      mFadeVolume(0.0f) {
    _LoadMusic();
}

void MusicComponent::OnCreate() {}

void MusicComponent::OnDestroy() {
    StopMusic();
}

void MusicComponent::OnEnable() {
    PlayMusic();
}

void MusicComponent::OnDisable() {
    PauseMusic();
}

void MusicComponent::OnUpdate(double time_diff) {
    if(mFadeFlag) {
        auto* resmgr = ResourceManager::Get();
        if(mElapsedTime >= mFadeTime) {
            mFadeFlag = false;
        } else {
            float volume = ((time_diff / mFadeTime) * mFadeVolume) + resmgr->GetMusicFile(mMusicFileName)->GetVolume();
            resmgr->GetMusicFile(mMusicFileName)->SetVolume(volume);
            mElapsedTime += time_diff;
        }
    }
}

void MusicComponent::SetMusicFileName(const QString& music_file_name) {
    if(music_file_name != mMusicFileName && IsCreated()) {
        // we got a new music; load it
        _LoadMusic();
    }
    mMusicFileName = music_file_name;
}

const QString& MusicComponent::GetMusicFileName() const {
    return mMusicFileName;
}

void MusicComponent::SetVolume(float volume) {
    if(ResourceManager::Get()->GetMusicFile(mMusicFileName)->GetVolume() != volume) {
        ResourceManager::Get()->GetMusicFile(mMusicFileName)->SetVolume(volume);
        emit VolumeChanged(volume);
    }
}

void MusicComponent::SetMasterVolume(float volume) {
    if(sf::Listener::GetGlobalVolume() != volume) {
        sf::Listener::SetGlobalVolume(volume);
        emit MasterVolumeChanged(volume);
    }
}

void MusicComponent::_LoadMusic() {
    if(mMusicFileName == "") {
        Logger::Get().Error("MusicComponent [" + mName + "]: Needs a music file.");
    }
    if(!ResourceManager::Get()->AddMusicFile(mMusicFileName)) {
        Logger::Get().Error("MusicComponent [" + mName + "]: Wasn't able to load music file [" + mMusicFileName + "].");
    }
}

void MusicComponent::PlayMusic() {
    if(ResourceManager::Get()->GetMusicFile(mMusicFileName)->GetStatus() != sf::Music::Playing) {
        // play music if possible
        ResourceManager::Get()->GetMusicFile(mMusicFileName)->Play();
        emit MusicPlayed();
    }
}

void MusicComponent::StopMusic() {
    if(ResourceManager::Get()->GetMusicFile(mMusicFileName)->GetStatus() != sf::Music::Stopped) {
        // stop music if possible
        ResourceManager::Get()->GetMusicFile(mMusicFileName)->Stop();
        emit MusicStopped();
    }
}

void MusicComponent::PauseMusic() {
    if(ResourceManager::Get()->GetMusicFile(mMusicFileName)->GetStatus() != sf::Music::Paused) {
        // pause music if possible
        ResourceManager::Get()->GetMusicFile(mMusicFileName)->Pause();
        emit MusicPaused();
    }
}

void MusicComponent::Fade(double time, float target_volume) {
    mFadeFlag = true;
    if(time > 0.0)
        mFadeTime = time;
    else
        mFadeTime = 0.001;
    mElapsedTime = 0.0;
    // Keep the range of the volume between 0.0 and 100.0
    if(target_volume < 0.0f)
        target_volume = 0.0f;
    else if(target_volume > 100.0f)
        target_volume = 100.0f;
    mFadeVolume = target_volume - ResourceManager::Get()->GetMusicFile(mMusicFileName)->GetVolume();
}
}
