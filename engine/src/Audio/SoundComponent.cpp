
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

SoundComponent::SoundComponent(const QString sound_file_name, const QString name)
    : Component(name),
      mSoundFileName(sound_file_name) {
    _loadSound();
}

void SoundComponent::onInitialize() {}

void SoundComponent::onDeinitialize() {}

void SoundComponent::onUpdate(double time_diff) {
    mSound.setPosition(mNode->getPosition(Node::SCENE).x,
                       mNode->getPosition(Node::SCENE).y,
                       mNode->getPosition(Node::SCENE).z);
}

void SoundComponent::onSerialize(IOPacket& packet) {
    packet.stream(mSoundFileName, "sound_file");
}

void SoundComponent::setSoundFileName(const QString sound_file_name) {
    if(sound_file_name != mSoundFileName && isInitialized()) {
        // we got a new sound; load it
        _loadSound();
    }
    mSoundFileName = sound_file_name;
}

const QString SoundComponent::getSoundFileName() const {
    return mSoundFileName;
}

sf::Sound& SoundComponent::getSound() {
	return mSound;
}

void SoundComponent::playSound() {
    if(mSound.getStatus() != sf::Sound::Playing) {
        // play sound if possible and enabled
        if(isEnabled())
            mSound.play();
        emit soundPlayed();
    }
}

void SoundComponent::pauseSound() {
    if(mSound.getStatus() != sf::Sound::Paused) {
        // pause sound if possible
        mSound.pause();
        emit soundPaused();
    }
}

void SoundComponent::stopSound() {
    if(mSound.getStatus() != sf::Sound::Stopped) {
        // stop sound if possible
        mSound.stop();
        emit soundStopped();
    }
}

void SoundComponent::_loadSound() {
    if(mSoundFileName == "") {
        Logger::get().error("SoundComponent [" + mName + "]: Needs a sound file.");
    }
    if(!ResourceManager::get()->addSoundBuffer(mSoundFileName)) {
        Logger::get().error("SoundComponent [" + mName + "]: Wasn't able to load sound file [" + mSoundFileName + "].");
    } else {
        mSound.setBuffer(*ResourceManager::get()->getSoundBuffer(mSoundFileName));
    }
}

void SoundComponent::setVolume(float volume) {
    if(mSound.getVolume() != volume) {
        mSound.setVolume(volume);
        emit volumeChanged(volume);
    }
}

void SoundComponent::setMasterVolume(float volume) {
    if(sf::Listener::getGlobalVolume() != volume) {
        sf::Listener::setGlobalVolume(volume);
        emit masterVolumeChanged(volume);
    }
}

}
