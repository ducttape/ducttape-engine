#ifndef DUCTTAPE_ENGINE_COMPONENT_SOUNDCOMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_SOUNDCOMPONENTLISTENER

#include "ComponentListener.hpp"

namespace dt {

/**
 * The ComponentListener for the SoundComponent.
 * @see SoundComponent
 */
class SoundComponentListener : public ComponentListener {
public:
    /**
     * Callback called when SoundComponent registers a pausing Event.
     */
    void Paused();

    /**
     * Callback called when SoundComponent registers a pausing Event.
     */
    virtual void OnPaused();
};

}

#endif
