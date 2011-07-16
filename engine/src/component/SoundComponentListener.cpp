#include "SoundComponentListener.hpp"

namespace dt {

void SoundComponentListener::Paused() {
    _CallSignal("Paused");
    OnPaused();
}

void SoundComponentListener::OnPaused() {}

}
