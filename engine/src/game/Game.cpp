
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Game.hpp"

namespace dt {

Game::Game() {
    mIsShutdownRequested = false;
    mIsRunning = false;
}

void Game::Run(int argc, char** argv) {
    Root& root = Root::get_mutable_instance();

    root.Initialize(argc, argv);
    OnInitialize();

    mClock.Reset();
    mIsRunning = true;
    while(!mIsShutdownRequested) {
        // TIMING
        // TODO: Implement real timing instead of just getting the time difference
        double frame_time = mClock.GetElapsedTime() / 1000.0;
        mClock.Reset();

        Root::get_mutable_instance().GetEventManager()->HandleEvent(new BeginFrameEvent(frame_time));

        // INPUT
        // InputManager::Capture();
        // TODO: Implement InputManager.

        // NETWORKING
        root.GetNetworkManager()->HandleIncomingEvents();
        root.GetNetworkManager()->SendQueuedEvents();

        // DISPLAYING
        // Won't work without a CameraComponent which initializes the render system!
        root.GetDisplayManager()->Render();

        // TODO: Simulate something more expensive -> ~ 200 FPS
        sf::Sleep(5);
    }

    // Send the GoodbyeEvent to close the network connection.
    root.GetEventManager()->HandleEvent(new GoodbyeEvent("The client closed the session."));
    root.GetNetworkManager()->SendQueuedEvents();

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
