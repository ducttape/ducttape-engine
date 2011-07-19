
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_MUSICCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_MUSICCOMPONENT

#include <SFML/Audio.hpp>

#include "component/Component.hpp"
#include "Root.hpp"

namespace dt {

/**
  * Adds music to the scene.
  */
class MusicComponent : public Component {
public:
    /**
     * Advanced constructor.
     * @see Component
     */
    MusicComponent(const std::string& name, const std::string& music_file = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(float time_diff);

    /**
      * Sets the file to load music from.
      * @param music_file The file to load the music from.
      */
    void SetMusicFile(const std::string& music_file);

    /**
      * Returns the file the music was loaded from.
      * @returns The file the music was loaded from.
      */
    const std::string& GetMusicFile() const;

private:
    /**
      * Private method. Prepares the music stream.
      */
    void _LoadMusic();

    /**
      * Private method. Plays the music stream.
      */
    void _PlayMusic();

    /**
      * Private method. Stops the music stream.
      */
    void _StopMusic();

    std::string mMusicFile;     //!< The file the music was loaded from.

};

}

#endif
