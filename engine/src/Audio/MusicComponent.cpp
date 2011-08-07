
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Audio/MusicComponent.hpp>

#include <Audio/MusicControlEvent.hpp>
#include <Core/ResourceManager.hpp>
#include <Utils/Logger.hpp>

#include <SFML/Audio/Listener.hpp>

namespace dt {

MusicComponent::MusicComponent(const QString& music_file, const QString& name)
    : Component(name),
      mMusicFile(music_file),
      mFadeFlag(false),
      mElapsedTime(0.0),
      mFadeTime(0.0),
      mFadeVolume(0.0f) {
    _LoadMusic();
}

void MusicComponent::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_MUSICCONTROLEVENT") {
        std::shared_ptr<MusicControlEvent> m = std::dynamic_pointer_cast<MusicControlEvent>(e);
        if(m->GetAction() == MusicControlEvent::PAUSE) {
            PauseMusic();
        } else if(m->GetAction() == MusicControlEvent::STOP) {
            StopMusic();
        } else if(m->GetAction() == MusicControlEvent::PLAY) {
            PlayMusic();
        }
    }
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
        }
        else {
            float volume = ((time_diff / mFadeTime) * mFadeVolume) + resmgr->GetMusicFile(mMusicFile)->GetVolume();
            resmgr->GetMusicFile(mMusicFile)->SetVolume(volume);
            mElapsedTime += time_diff;
        }
    }
}

void MusicComponent::SetMusicFile(const QString& music_file) {
    if(music_file != mMusicFile && IsCreated()) {
        // we got a new music; load it
        _LoadMusic();
    }
    mMusicFile = music_file;
}

const QString& MusicComponent::GetMusicFile() const {
    return mMusicFile;
}

void MusicComponent::SetVolume(float volume) {
    ResourceManager::Get()->GetMusicFile(mMusicFile)->SetVolume(volume);
}

void MusicComponent::SetMasterVolume(float volume) {
    sf::Listener::SetGlobalVolume(volume);
}

void MusicComponent::_LoadMusic() {
    if(mMusicFile == "") {
        Logger::Get().Error("MusicComponent [" + mName + "]: Needs a music file.");
    }
    if(!ResourceManager::Get()->AddMusicFile(mMusicFile)) {
        Logger::Get().Error("MusicComponent [" + mName + "]: Wasn't able to load music file [" + mMusicFile + "].");
    }
}

void MusicComponent::PlayMusic() {
    // play music if possible
    ResourceManager::Get()->GetMusicFile(mMusicFile)->Play();
}

void MusicComponent::StopMusic() {
    // stop music if possible
    ResourceManager::Get()->GetMusicFile(mMusicFile)->Stop();
}

void MusicComponent::PauseMusic() {
    // pause music if possible
    ResourceManager::Get()->GetMusicFile(mMusicFile)->Pause();
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
    mFadeVolume = target_volume - ResourceManager::Get()->GetMusicFile(mMusicFile)->GetVolume();
}
}
