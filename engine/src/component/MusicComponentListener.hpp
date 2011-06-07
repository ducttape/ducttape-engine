#ifndef DUCTTAPE_ENGINE_COMPONENT_MUSICCOMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_MUSICCOMPONENTLISTENER

#include "ComponentListener.hpp"

namespace dt {

/**
 * The ComponentListener for the MusicComponent.
 * @see MusicComponent
 */
class MusicComponentListener : public ComponentListener {
public:
    /**
     * Callback called when MusicComponent registers a pausing Event.
     */
    void Paused();

    /**
     * Callback called when MusicComponent registers a pausing Event.
     */
    virtual void OnPaused();
};

}

#endif
