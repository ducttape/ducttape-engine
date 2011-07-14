#ifndef DUCTTAPE_ENGINE_COMPONENT_SOUNDCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_SOUNDCOMPONENT

#include <SFML/Audio.hpp>

#include "component/Component.hpp"
#include "component/SoundComponentListener.hpp"
#include "Root.hpp"

namespace dt {

/**
  * Component for managing sounds in 3D enviroment.
  * @todo Positions should be calculated relatively to position of player, but there is no method to get those now.
  */
class SoundComponent : public Component {
public:
    /**
     * Advanced constructor.
     * @see Component
     */
    SoundComponent(const std::string& name, const std::string& sound_file = "", SoundComponentListener* custom_listener = new SoundComponentListener());

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate(float time_diff);

	/**
     * Plays the sound located in mSound.
	 * @param name of sound file to add. To get sound in 3D file should be mono.
     */
    void SetSoundFile(const std::string& sound_file);

	/**
     * Returns sound file name.
     */
    const std::string& GetSoundFile() const;

	/**
     * Returns mSound for default action access.
     */
	sf::Sound& GetSound();

private:
	/**
     * Private method. Tries to load sound to buffer and then from buffer to mSound.
     */
    void _LoadSound();

	/**
     * Private method. Tries to play sound located in mSound.
     */
    void _PlaySound();

	/**
     * Private method. Tries to stop playing of sound located in mSound.
     */
    void _StopSound();

    std::string mSoundFile; //!< Name of sound file loaded in component.
	sf::Sound mSound; //!< SFML Sound. It is set during _LoadSound().

};

}

#endif
