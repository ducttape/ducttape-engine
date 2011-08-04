
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/ResourceManager.hpp>

#include <Core/Root.hpp>

namespace dt {

ResourceManager::ResourceManager()
    : mDataPathsSearched(false) {}


ResourceManager::~ResourceManager() {}

void ResourceManager::Initialize() {
    _FindDataPaths();
}

void ResourceManager::Deinitialize() {}

ResourceManager* ResourceManager::Get() {
    return Root::GetInstance().GetResourceManager();
}

void ResourceManager::AddResourceLocation(const QString& path, const QString& type, bool recursive) {
    boost::filesystem::path file(path.toStdString());

    // Does the path exist?
    if(_FindFileInDataPaths(file)) {
        DisplayManager::Get()->CreateOgreRoot();
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(file.string(), type.toStdString(),
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, recursive);
    }
}

bool ResourceManager::AddSoundBuffer(const QString& path, const QString& sound_file) {
    boost::filesystem::path file(path.toStdString());

    // Does the path exist?
    if(_FindFileInDataPaths(file)) {
        // if the optional param key is not given, use the basename as key
        QString sound_key;
        if(sound_file == "") {
            sound_key = path;
        } else {
            sound_key = sound_file;
        }

        // if a sound with that key already exists in the dictionary, return
        if(mSoundBuffers.count(sound_key) != 0) {
            return true;
        }

        sf::SoundBuffer sound_buffer;
        if(!sound_buffer.LoadFromFile(file.string())) {
            Logger::Get().Error("Loading " + QString::fromStdString(file.string()) + " failed.");
            return false;
        }

        mSoundBuffers[sound_key] = sound_buffer;
        return true;
    }
    return false;
}

const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const QString& sound_file) {
	if(mSoundBuffers.count(sound_file) >= 1) {
		return mSoundBuffers[sound_file];
	} else {
        Logger::Get().Error("Tried getting  " + sound_file + " but this sound doesn't exist!");
		exit(1);
	}
}

bool ResourceManager::AddMusicFile(const QString& path, const QString& music_file) {
    boost::filesystem::path file(path.toStdString());

    // Does the path exist?
    if(_FindFileInDataPaths(file)) {
        // if the optional param music_file is not given, use the basename as key
        QString music_key;
        if(music_file == "") {
            music_key = path;
        } else {
            music_key = music_file;
        }

        // if a sound with that key already exists in the dictionary, return
        if(mMusic.count(music_key) != 0) {
            return true;
        }

        std::shared_ptr<sf::Music> music(new sf::Music());
        if(!music->OpenFromFile(file.string())) {
            Logger::Get().Error("Loading " + QString::fromStdString(file.string()) + " failed.");
            return false;
        }
        mMusic[music_key] = music;
        return true;
    }
    return false;
}

std::shared_ptr<sf::Music> ResourceManager::GetMusicFile(const QString& music_file) {
	if(mMusic.count(music_file) >= 1) {
		return mMusic[music_file];
	} else {
        Logger::Get().Error("Tried getting  " + music_file + " but this music doesn't exist!");
		exit(1);
	}
}

bool ResourceManager::FoundDataPath() const {
    return mDataPaths.size() > 0;
}

void ResourceManager::AddDataPath(boost::filesystem::path path) {
    // make sure the default paths are checked first
    if(!mDataPathsSearched)
        _FindDataPaths();

    if(boost::filesystem::is_directory(path)) {
        Logger::Get().Debug("Added data path: " + QString::fromStdString(path.string()));
        mDataPaths.push_back(path);
    } else {
        Logger::Get().Warning("Cannot add data path: " + QString::fromStdString(path.string()) + ". Not a directory.");
    }
}

void ResourceManager::AddDataPath(QString path) {
    AddDataPath(boost::filesystem::path(path.toStdString()));
}

void ResourceManager::_FindDataPaths() {
    mDataPathsSearched = true;

    // check recursively upwards
    boost::filesystem::path path(Root::GetInstance().GetExecutablePath());
    while(path.has_parent_path()) {
        path = path.parent_path();
        if(boost::filesystem::is_directory(path / "data")) {
            AddDataPath(path / "data");
            break;
        }
    }

    // check compiled-in path (DT_DATA_PATH)
#ifdef DT_DATA_PATH
    boost::filesystem::path dt_data_path(DT_DATA_PATH);
    if(boost::filesystem::is_directory(dt_data_path)) {
        AddDataPath(dt_data_path);
    } else {
        Logger::Get().Error("Compiled-in data path not found: " + dt_data_path.string());
    }
#endif
}

bool ResourceManager::_FindFileInDataPaths(boost::filesystem::path& file) {
    const boost::filesystem::path f(file);

    for(auto iter = mDataPaths.begin(); iter != mDataPaths.end(); ++iter) {
        if(boost::filesystem::exists(*iter / f)) {
            file = *iter / f;
            return true;
        }
    }
    Logger::Get().Error("Cannot find file or directory: " + QString::fromStdString(file.string()));
    return false;
}

}
