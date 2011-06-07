#include "MusicComponent.hpp"

#include "utils/Logger.hpp"

namespace dt {

MusicComponent::MusicComponent(const std::string& name, const std::string& music_file, MusicComponentListener* custom_listener)
    : Component(name, custom_listener) {
    mMusicFile = music_file;
    _LoadMusic();
}

void MusicComponent::HandleEvent(Event* e) {
    auto resmgr = Root::get_mutable_instance().GetResourceManager();
    if(e->GetType() == "DT_MUSICPAUSEEVENT") {
        if(resmgr->GetMusicFile(mMusicFile)->GetStatus() == sf::Music::Paused) {
            resmgr->GetMusicFile(mMusicFile)->Play();
        } else {
            resmgr->GetMusicFile(mMusicFile)->Pause();
        }
    }
}

void MusicComponent::OnActivate() {
   _PlayMusic();
}

void MusicComponent::OnDeactivate() {
    _StopMusic();
}

void MusicComponent::OnUpdate() {

}

void MusicComponent::SetMusicFile(const std::string& music_file) {
    if(music_file != mMusicFile && IsActivated()) {
        // we got a new music; load it
        _LoadMusic();
    }
    mMusicFile = music_file;
}

const std::string& MusicComponent::GetMusicFile() const {
    return mMusicFile;
}

void MusicComponent::_LoadMusic() {
    // stop existing music
    _StopMusic();

    if(mMusicFile == "") {
        Logger::Get().Error("MusicComponent [" + mName + "]: Needs a music file.");
    }
    if(!Root::get_mutable_instance().GetResourceManager()->AddMusicFile(mMusicFile)) {
        Logger::Get().Error("MusicComponent [" + mName + "]: Wasn't able to load music file [" + mMusicFile + "].");
    }
}

void MusicComponent::_PlayMusic() {
    // play music if possible
    Root::get_mutable_instance().GetResourceManager()->GetMusicFile(mMusicFile)->Play();
}

void MusicComponent::_StopMusic() {
    // stop music if possible
    Root::get_mutable_instance().GetResourceManager()->GetMusicFile(mMusicFile)->Stop();
}

}
