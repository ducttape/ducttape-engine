
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "resources/ResourceManager.hpp"
#include "utils/Logger.hpp"

#include "Root.hpp"

namespace dt {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::Initialize() {
    _FindDataPath();
}

void ResourceManager::Deinitialize() {}

void ResourceManager::AddResourceLocation(const boost::filesystem::path& path, const std::string& type, bool recursive) {
    auto full_path = mDataPath / path;

    // Does the path exist?
    if(!boost::filesystem::exists(full_path)) {
        Logger::Get().Error("Path \""+full_path.string()+"\" not found.");
    }

    Root::get_mutable_instance().GetDisplayManager()->CreateOgreRoot();
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(full_path.string(), type, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, recursive);
}

bool ResourceManager::AddSoundBuffer(const boost::filesystem::path& path, const std::string& sound_file) {
    auto full_path = mDataPath / path;

    // Does the path exist?
	if(!boost::filesystem::is_regular_file(full_path)) {
        Logger::Get().Error("Path \""+full_path.string()+"\" not found.");
	}

    // if the optional param key is not given, use the basename as key
    std::string sound_key = "";
    if(sound_file == "") {
        sound_key = (path.parent_path() / path.filename()).string();
    } else {
        sound_key = sound_file;
    }

    // if a sound with that key already exists in the dictionary, return
    if(mSoundBuffers.count(sound_key) != 0) {
        return true;
    }

	sf::SoundBuffer sound_buffer;
	if(!sound_buffer.LoadFromFile(full_path.string())) {
        Logger::Get().Error("Loading \""+full_path.string()+"\" failed.");
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
    auto full_path = mDataPath / path;

    // Does the path exist?
    if(!boost::filesystem::is_regular_file(full_path)) {
        Logger::Get().Error("Path \""+full_path.string()+"\" not found.");
    }

    // if the optional param music_file is not given, use the basename as key
    std::string music_key = "";
    if(music_file == "") {
        music_key = (path.parent_path() / path.filename()).string();
    } else {
        music_key = music_file;
    }

    // if a sound with that key already exists in the dictionary, return
    if(mMusic.count(music_key) != 0) {
        return true;
    }

    boost::shared_ptr<sf::Music> music(new sf::Music());
    if(!music->OpenFromFile(full_path.string())) {
        Logger::Get().Error("Loading \""+full_path.string()+"\" failed.");
    }
    mMusic[music_key] = music;

    return true;
}

boost::shared_ptr<sf::Music> ResourceManager::GetMusicFile(const std::string& music_file) {
	if(mMusic.count(music_file) >= 1) {
		return mMusic[music_file];
	} else {
        Logger::Get().Error("Tried getting  \""+music_file+"\" but this sound doesn't exist!");
		exit(1);
	}
}

void ResourceManager::_FindDataPath() {
    bool found_data_path = false;

    auto path = Root::get_const_instance().GetExecutablePath();
    while(path.has_parent_path() && !found_data_path) {
        path = path.parent_path();
        Logger::Get().Debug("Trying to find data path in \""+path.string()+"\"");

        if(boost::filesystem::is_directory(path / "data")) {
            found_data_path = true;
            mDataPath = path / "data";
            Logger::Get().Info("Found data path: \""+mDataPath.string()+"\"");
        }
    }

    if(!found_data_path) {
        Logger::Get().Error("Unable to find data path");
        exit(1);
    }
}

}
