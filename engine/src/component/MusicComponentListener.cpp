#include "MusicComponentListener.hpp"

namespace dt {

void MusicComponentListener::Paused() {
    _CallSignal("Paused");
    OnPaused();
}

void MusicComponentListener::OnPaused() {}

}
