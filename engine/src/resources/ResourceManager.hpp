
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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
  * @todo This manager should eventually only allow adding of locations which are managed automatically.
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
      * @param sound_file Optional name for the SoundBuffer. If none is given, the filename is used.
      * @see http://www.sfml-dev.org/documentation/2.0/classsf_1_1SoundBuffer.php#aec8a8960c145915855d956600e9c7032
      * @todo Merge this into AddResourceLocation
      */
	bool AddSoundBuffer(const boost::filesystem::path& path, const std::string& sound_file="");

    /**
      * Retrieves a single sound buffer from memory. If the requested soundbuffer is not found,
      * an error is printed and the program will exit with status 1.
      * @param sound_file Identifier of the sound buffer to get.
      * @returns A reference to the requested sound buffer.
      * @todo This shouldn't really be required if resources or loaded automatically in a lazy manner.
      */
    const sf::SoundBuffer& GetSoundBuffer(const std::string& sound_file);

    /**
     * Adds a single music file to memory.
     * @todo Merge this into AddResourceLocation
     */
    bool AddMusicFile(const boost::filesystem::path& path, const std::string& music_file="");

    /**
     * Retrieves a single music file from memory.
     */
    std::shared_ptr<sf::Music> GetMusicFile(const std::string& music_file);
 
private:
    /**
     * Private method for internal use only. Tries to find the data path using
     * two different strategies in this order:
     * -# Starting at the executable path, go up the hierarchy and test each
     *  path for the existence of data/ (this is convenient for developers)
     * -# Check compile-time path set by DATA_PATH (this is for system installation)
     *  @todo Only the first method works for now.
     */
    void _FindDataPath();

    boost::filesystem::path mDataPath; //!< Path to the actual data directory. It is set during Initialize().
    std::map<std::string, std::shared_ptr<sf::Music>> mMusic; //!< Pool of registered music objects. This does not actually contain the music data since music is actually streamed.
	boost::ptr_map<std::string, sf::SoundBuffer> mSoundBuffers; //!< Pool of registered sound buffers. These are in fact loaded into memory.
};

}

#endif
