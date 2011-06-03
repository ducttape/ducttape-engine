#ifndef DUCTTAPE_ENGINE_GAME_GAME
#define DUCTTAPE_ENGINE_GAME_GAME

#include "Root.hpp"

namespace dt {

class Game {
public:
    Game();
    virtual void OnInitialize() = 0;
    void Run();

    void RequestShutdown();
    bool IsRunning();

protected:
    bool mIsShutdownRequested;
    bool mIsRunning;
};

}

#endif
