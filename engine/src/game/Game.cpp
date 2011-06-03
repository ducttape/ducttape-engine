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

    mClock.Reset();
    mIsRunning = true;
    while(!mIsShutdownRequested) {
        // TIMING
        // TODO: Implement real timing instead of just getting the time difference
        uint32_t frame_time = mClock.GetElapsedTime();
        mClock.Reset();

        Root::get_mutable_instance().GetEventManager()->HandleEvent(new BeginFrameEvent(frame_time));

        // INPUT
        // InputManager::Capture();
        // TODO: Implement InputManager.

        // NETWORKING
        root.GetNetworkManager()->HandleIncomingEvents();
        root.GetNetworkManager()->SendQueuedEvents();

        // DISPLAYING
        // TODO: Implement DisplayManager.

        // TODO: Simulate something more expensive -> ~ 200 FPS
        sf::Sleep(5);
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
