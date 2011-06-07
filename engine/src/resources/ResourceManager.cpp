#include "resources/ResourceManager.hpp"
#include "utils/Logger.hpp"

#include "Root.hpp"

namespace dt {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::Initialize() {}

void ResourceManager::Deinitialize() {}

void ResourceManager::AddResourceLocation(const boost::filesystem::path& path, const std::string& type, bool recursive) {
    if(!boost::filesystem::exists(path)) {
        Logger::Get().Error("Path \""+path.string()+"\" not found.");
    }

    Root::get_mutable_instance().GetDisplayManager()->CreateOgreRoot();
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path.string(), type, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, recursive);
}

bool ResourceManager::AddSoundBuffer(const boost::filesystem::path& path, const std::string& sound_file) {
	if(!boost::filesystem::is_regular_file(path)) {
        Logger::Get().Error("Path \""+path.string()+"\" not found.");
	}

    // if the optional param key is not given, use the basename as key
    std::string sound_key = "";
    if(sound_file == "") {
        sound_key = boost::filesystem::basename(path.string());
    } else {
        sound_key = sound_file;
    }

    // if a sound with that key already exists in the dictionary, return
    if(mSoundBuffers.count(sound_key) != 0) {
        return true;
    }

	sf::SoundBuffer sound_buffer;
	if(!sound_buffer.LoadFromFile(path.string())) {
        Logger::Get().Error("Loading \""+path.string()+"\" failed.");
	}

	mSoundBuffers[sound_key] = sound_buffer;

    return true;
}

const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& sound_file) {
	if(mSoundBuffers.count(sound_file) >= 1) {
		return mSoundBuffers[sound_file];
	} else {
        Logger::Get().Error("Tried getting  \""+sound_file+"\" but this sound doesn't exist!");
		exit(1);
	}
}
bool ResourceManager::AddMusicFile(const boost::filesystem::path& path, const std::string& music_file) {
    if(!boost::filesystem::is_regular_file(path)) {
        Logger::Get().Error("Path \""+path.string()+"\" not found.");
    }

    // if the optional param key is not given, use the basename as key
    std::string music_key = "";
    if(music_file == "") {
        music_key = boost::filesystem::basename(path.string());
    } else {
        music_key = music_file;
    }

    // if a sound with that key already exists in the dictionary, return
    if(mMusic.count(music_key) != 0) {
        return true;
    }

    sf::Music music;
    if(!music.OpenFromFile(path.string())) {
        Logger::Get().Error("Loading \""+path.string()+"\" failed.");
    }
    mMusic[music_key] = &music;

    return true;
}

sf::Music* ResourceManager::GetMusicFile(const std::string& music_file) {
	if(mMusic.count(music_file) >= 1) {
		return mMusic[music_file];
	} else {
        Logger::Get().Error("Tried getting  \""+music_file+"\" but this sound doesn't exist!");
		exit(1);
	}
}


}
