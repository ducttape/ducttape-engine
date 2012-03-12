
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

MusicComponent::MusicComponent(const QString music_file_name, const QString name)
    : Component(name),
      mMusicFileName(music_file_name),
      mFadeFlag(false),
      mElapsedTime(0.0),
      mFadeTime(0.0),
      mFadeVolume(0.0f) {
    _loadMusic();
}

void MusicComponent::onInitialize() {}

void MusicComponent::onDeinitialize() {
    stopMusic();
}

void MusicComponent::onEnable() {
    playMusic();
}

void MusicComponent::onDisable() {
    pauseMusic();
}

void MusicComponent::onUpdate(double time_diff) {
    if(mFadeFlag) {
        auto* resmgr = ResourceManager::get();
        if(mElapsedTime >= mFadeTime) {
            mFadeFlag = false;
        } else {
            float volume = ((time_diff / mFadeTime) * mFadeVolume) + resmgr->getMusicFile(mMusicFileName)->getVolume();
            resmgr->getMusicFile(mMusicFileName)->setVolume(volume);
            mElapsedTime += time_diff;
        }
    }
}

void MusicComponent::onSerialize(IOPacket& packet) {
    packet.stream(mMusicFileName, "music_file");
}

void MusicComponent::setMusicFileName(const QString music_file_name) {
    if(music_file_name != mMusicFileName && isInitialized()) {
        // we got a new music; load it
        _loadMusic();
    }
    mMusicFileName = music_file_name;
}

const QString MusicComponent::getMusicFileName() const {
    return mMusicFileName;
}

void MusicComponent::setVolume(float volume) {
    if(ResourceManager::get()->getMusicFile(mMusicFileName)->getVolume() != volume) {
        ResourceManager::get()->getMusicFile(mMusicFileName)->setVolume(volume);
        emit volumeChanged(volume);
    }
}

void MusicComponent::setMasterVolume(float volume) {
    if(sf::Listener::getGlobalVolume() != volume) {
        sf::Listener::setGlobalVolume(volume);
        emit masterVolumeChanged(volume);
    }
}

void MusicComponent::_loadMusic() {
    if(mMusicFileName == "") {
        Logger::get().error("MusicComponent [" + mName + "]: Needs a music file.");
    }
    if(!ResourceManager::get()->addMusicFile(mMusicFileName)) {
        Logger::get().error("MusicComponent [" + mName + "]: Wasn't able to load music file [" + mMusicFileName + "].");
    }
}

void MusicComponent::playMusic() {
    if(ResourceManager::get()->getMusicFile(mMusicFileName)->getStatus() != sf::Music::Playing) {
        // play music if possible
        ResourceManager::get()->getMusicFile(mMusicFileName)->play();
        emit musicPlayed();
    }
}

void MusicComponent::stopMusic() {
    if(ResourceManager::get()->getMusicFile(mMusicFileName)->getStatus() != sf::Music::Stopped) {
        // stop music if possible
        ResourceManager::get()->getMusicFile(mMusicFileName)->stop();
        emit musicStopped();
    }
}

void MusicComponent::pauseMusic() {
    if(ResourceManager::get()->getMusicFile(mMusicFileName)->getStatus() != sf::Music::Paused) {
        // pause music if possible
        ResourceManager::get()->getMusicFile(mMusicFileName)->pause();
        emit musicPaused();
    }
}

void MusicComponent::fade(double time, float target_volume) {
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
    mFadeVolume = target_volume - ResourceManager::get()->getMusicFile(mMusicFileName)->getVolume();
}
}
