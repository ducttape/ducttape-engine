#ifndef DUCTTAPE_ENGINE_RESOURCES_RESOURCEMANAGER
#define DUCTTAPE_ENGINE_RESOURCES_RESOURCEMANAGER

#include <SFML/Audio.hpp>

#include <iostream>
#include <queue>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/filesystem.hpp>

#include <OgreResourceGroupManager.h>

namespace dt {

/**
  * Manager for loading resources to memory and communicating resources to Ogre.
  */
class ResourceManager {
public:
    /**
      * Default constructor.
      */
    ResourceManager();

    /**
      * Default destructor.
      */
    ~ResourceManager();

    /**
      * Initializer. 
      */
    void Initialize();

    /**
      *  Deinitializer.
      */
    void Deinitialize();

    /**
      * Adds a resource location of to the search path.
      * @param path Path of the resource. Depending on \a type this is either a directory or a single file.
      * @param type Can either be \c FileSystem for a directory in the filesystem sys or \c Zip for a zip file containing the resources.
      * @param recursive A flag to set when resources should be searched recursively.
      * @todo Perhaps merge the other resource methods into this.
      */
	void AddResourceLocation(const boost::filesystem::path& path, const std::string& type, bool recursive = false);

    /**
      * Adds a single sound file to memory. A sound file in memory is called a sound buffer.
      * @param path Path of the SoundBuffer. For a complete list of supported formats, see SFML documentation.
      * @param key Optional name for the SoundBuffer. If none is given, the filename is used.
      * @see http://www.sfml-dev.org/documentation/2.0/classsf_1_1SoundBuffer.php#aec8a8960c145915855d956600e9c7032
      * @todo Merge this into AddResourceLocation
      */
	bool AddSoundBuffer(const boost::filesystem::path& path, const std::string& sound_key="");

    /**
      * Retrieves a single sound buffer from memory. If the requested soundbuffer is not found,
      * an error is printed and the program will exit with status 1.
      * @param sound Identifier of the sound buffer to get.
      * @returns A reference to the requested sound buffer.
      * @todo This shouldn't really be required if resources or loaded automatically in a lazy manner.
      */
    const sf::SoundBuffer& GetSoundBuffer(const std::string& sound_key);
 
private:
	boost::ptr_map<std::string, sf::Music> mMusic;
	boost::ptr_map<std::string, sf::SoundBuffer> mSoundBuffers;
};

}

#endif
