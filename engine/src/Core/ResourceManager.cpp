
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/ResourceManager.hpp>

#include <Core/Root.hpp>
#include <Utils/Utils.hpp>
#include <Utils/LogManager.hpp>
#include <Graphics/DisplayManager.hpp>

#include <QCoreApplication>

#include <Ogre.h>

namespace dt {

ResourceManager::ResourceManager()
    : mDataPathsSearched(false) {}


ResourceManager::~ResourceManager() {}

void ResourceManager::initialize() {
    _findDataPaths();
}

void ResourceManager::deinitialize() {}

ResourceManager* ResourceManager::get() {
    return Root::getInstance().getResourceManager();
}

void ResourceManager::addResourceLocation(const QString path, const QString type, bool recursive) {
    QFile file(findFile(path).absoluteFilePath());

    // Does the path exist?
    if(file.exists()) {
        DisplayManager::get()->createOgreRoot();
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Utils::toStdString(file.fileName()), Utils::toStdString(type),
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, recursive);
    }
}

bool ResourceManager::addSoundBuffer(const QString path, const QString sound_file) {
    QFile file(findFile(path).absoluteFilePath());

    // Does the path exist?
    if(file.exists()) {
        // if the optional param key is not given, use the basename as key
        QString sound_key;
        if(sound_file == "") {
            sound_key = path;
        } else {
            sound_key = sound_file;
        }

        // if a sound with that key already exists in the dictionary, return
        if(mSoundBuffers.contains(sound_key)) {
            return true;
        }

        std::shared_ptr<sf::SoundBuffer> sound_buffer(new sf::SoundBuffer());
        if(!sound_buffer->loadFromFile(Utils::toStdString(file.fileName()))) {
            Logger::get().error("Loading sound <" + file.fileName() + "> failed.");
            return false;
        }

        mSoundBuffers.insert(sound_key, sound_buffer);
        return true;
    }
    return false;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(const QString sound_file) {
    if(mSoundBuffers.contains(sound_file)) {
        return mSoundBuffers.value(sound_file);
	} else {
        Logger::get().error("Error getting sound \"" + sound_file + "\": not found.");
		exit(1);
	}
}

bool ResourceManager::addMusicFile(const QString path, const QString music_file) {
    QFile file(findFile(path).absoluteFilePath());

    // Does the path exist?
    if(file.exists()) {
        // if the optional param music_file is not given, use the basename as key
        QString music_key;
        if(music_file == "") {
            music_key = path;
        } else {
            music_key = music_file;
        }

        // if a sound with that key already exists in the dictionary, return
        if(mMusic.contains(music_key)) {
            return true;
        }

        std::shared_ptr<sf::Music> music(new sf::Music());
        if(!music->openFromFile(Utils::toStdString(file.fileName()))) {
            Logger::get().error("Loading Music <" + file.fileName() + "> failed.");
            return false;
        }

        mMusic.insert(music_key, music);
        return true;
    }
    return false;
}

std::shared_ptr<sf::Music> ResourceManager::getMusicFile(const QString music_file) {
	if(mMusic.count(music_file) >= 1) {
		return mMusic[music_file];
	} else {
        Logger::get().error("Error getting music \"" + music_file + "\": not found.");
		exit(1);
	}
}

void ResourceManager::addDataPath(QDir path) {
    // make sure the default paths are checked first
    if(!mDataPathsSearched)
        _findDataPaths();

    if(path.exists()) {
        Logger::get().debug("Added data path: <" + path.absolutePath() + ">.");
        QDir::addSearchPath("data", path.absolutePath());
    } else {
        Logger::get().warning("Cannot add data path <" + path.path() + ">: not a directory.");
    }
}

QFileInfo ResourceManager::findFile(const QString relative_path) {
    QFile file("data:" + relative_path);

    QFileInfo info(file);

    if(!file.exists()) {
        Logger::get().error("Could not locate file <" + relative_path + ">.");
    }

    return info;
}

void ResourceManager::_findDataPaths() {
    mDataPathsSearched = true;

    // check recursively upwards
    QDir dir(QCoreApplication::applicationDirPath());

    while(!dir.isRoot()) {
        QDir data(dir.absolutePath() + "/data");
        if(data.exists()) {
            addDataPath(data.absolutePath());
            break;
        }
        dir.cdUp();
    }

    // check compiled-in path (DT_DATA_PATH)
#ifdef DT_DATA_PATH
    QDir data_path(DT_DATA_PATH);
    if(data_path.exists()) {
        AddDataPath(data_path.absolutePath());
    } else {
        Logger::Get().Error("Compiled-in data path <" + DT_DATA_PATH + "> not found.");
    }
#else
    // warn if we also did not find one previously
    if(dir.isRoot()) {
        // did not find the data path
        Logger::get().warning("No relative data path found.");
    }
#endif
}

}
