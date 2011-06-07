
void MusicManager::_LoadMusic() {
    // stop existing music
    _StopMusic();

    if(mMusicFile == "") {
        Logger::Get().Error("MusicComponent [" + mName + "]: Needs a music file.");
    }
    if(!Root::get_mutable_instance()->GetResourceManagerPtr()->AddMusicFile(mMusicFile)) {
        Logger::Get().Error("MusicComponent [" + mName + "]: Wasn't able to load music file [" + mMusicFile + "].");
    }
}

void MusicManager::_PlayMusic() {
    // play music if possible
    Root::get_mutable_instance()->GetResourceManager()->GetMusicFile(mMusicFile).Play();
}

void MusicManager::_StopMusic() {
    // stop music if possible
    Root::get_mutable_instance()->GetResourceManager()->GetMusicFile(mMusicFile).Stop();
}
