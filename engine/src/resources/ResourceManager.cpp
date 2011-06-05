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
        dt::Logger::Get().Error("Path \""+path.string()+"\" not found.");
    }

    Root::get_mutable_instance().GetDisplayManager()->CreateOgreRoot();
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path.string(), type, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, recursive);
}

bool ResourceManager::AddSoundBuffer(const boost::filesystem::path& path, const std::string& key) {
	if(!boost::filesystem::is_regular_file(path)) {
        dt::Logger::Get().Error("Path \""+path.string()+"\" not found.");
	}

    // if the optional param key is not given, use the basename as key
    std::string sound_key = "";
    if(key == "") {
        sound_key = boost::filesystem::basename(path.string());
    } else {
        sound_key = key;
    }

    // if a sound with that key already exists in the dictionary, return
    if(mSoundBuffers.count(sound_key) != 0) {
        return true;
    }

	sf::SoundBuffer sound_buffer;
	if(!sound_buffer.LoadFromFile(path.string())) {
        dt::Logger::Get().Error("Loading \""+path.string()+"\" failed.");
	}

	mSoundBuffers[sound_key] = sound_buffer;

    return true;
}

const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& sound_key) {
	if(mSoundBuffers.count(sound_key) >= 1) {
		return mSoundBuffers[sound_key];
	} else {
        dt::Logger::Get().Error("Tried getting  \""+sound_key+"\" but this sound doesn't exist!");
		exit(1);
	}
}

}
