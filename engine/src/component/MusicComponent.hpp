#ifndef DUCTTAPE_ENGINE_COMPONENT_MUSICCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_MUSICCOMPONENT

#include <SFML/Audio.hpp>

#include "component/Component.hpp"
#include "component/MusicComponentListener.hpp"
#include "Root.hpp"

namespace dt {

class MusicComponent : public Component {
public:
    /**
     * Advanced constructor.
     * @see Component
     */
    MusicComponent(const std::string& name, const std::string& music_file = "", MusicComponentListener* custom_listener = new MusicComponentListener());

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate();

    void SetMusicFile(const std::string& music_file);
    const std::string& GetMusicFile() const;

private:
    void _LoadMusic();
    void _PlayMusic();
    void _StopMusic();

    std::string mMusicFile;

};

}

#endif
