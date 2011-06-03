#include "Game.hpp"

namespace dt {

Game::Game() {
    mIsShutdownRequested = false;
    mIsRunning = false;
}

void Game::Run() {
    Root& root = Root::get_mutable_instance();

    root.Initialize();
    OnInitialize();

    mIsRunning = true;
    while(!mIsShutdownRequested) {
        // TIMING
        // TODO: Implement timing

        // INPUT
        // InputManager::Capture();
        // TODO: Implement InputManager.

        // NETWORKING
        root.GetNetworkManager()->HandleIncomingEvents();
        root.GetNetworkManager()->SendQueuedEvents();

        // DISPLAYING
        // TODO: Implement DisplayManager.
    }
    mIsRunning = false;

    root.Deinitialize();
}

void Game::RequestShutdown() {
    mIsShutdownRequested = true;
}

bool Game::IsRunning() {
    return mIsRunning;
}

}
