
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Game.hpp>

#include <Core/Root.hpp>
#include <Event/BeginFrameEvent.hpp>
#include <Network/GoodbyeEvent.hpp>
#include <Event/EventManager.hpp>
#include <Scene/StateManager.hpp>
#include <Input/InputManager.hpp>
#include <Network/NetworkManager.hpp>
#include <Graphics/DisplayManager.hpp>

#include <SFML/System/Sleep.hpp>

namespace dt {

Game::Game()
    : mIsShutdownRequested(false),
      mIsRunning(false) {}

void Game::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_WINDOWCLOSEDEVENT") {
        Logger::Get().Debug("The closed window triggered a game shutdown.");
        RequestShutdown();
    }
}

void Game::Run(State* start_state, int argc, char** argv) {
    Root& root = Root::GetInstance();

    root.Initialize(argc, argv);
    root.GetStateManager()->SetNewState(start_state);
    root.GetEventManager()->AddListener(this);

    mClock.Reset();
    mIsRunning = true;

    // read http://gafferongames.com/game-physics/fix-your-timestep for more
    // info about this timestep stuff, especially the accumulator and the
    // "spiral of death"
    double simulation_frame_time = 0.001;
    double accumulator = 0.0;
    sf::Clock anti_spiral_clock;

    while(!mIsShutdownRequested) {
        // TIMING
        // TODO: Implement real timing instead of just getting the time difference
        double frame_time = mClock.GetElapsedTime() / 1000.0;
        mClock.Reset();

        // Shift states and cancel if none are left
        if(!root.GetStateManager()->ShiftStates())
            break;

        // INPUT
        InputManager::Get()->Capture();

        accumulator += frame_time;
        while(accumulator >= simulation_frame_time) {
            anti_spiral_clock.Reset();
            // SIMULATION
            EventManager::Get()->
                InjectEvent(std::make_shared<BeginFrameEvent>(simulation_frame_time));

            // NETWORKING
            root.GetNetworkManager()->HandleIncomingEvents();
            root.GetNetworkManager()->SendQueuedEvents();

            double real_simulation_time = anti_spiral_clock.GetElapsedTime() / 1000.0;
            if(real_simulation_time > simulation_frame_time) {
                // this is bad! the simulation did not render fast enough
                // to have some time left for rendering etc.

                // skip a frame to catch up
                accumulator -= simulation_frame_time;

                accumulator -= real_simulation_time;
            }
            accumulator -= simulation_frame_time;
        }

        // DISPLAYING
        // Won't work without a CameraComponent which initializes the render system!
        root.GetDisplayManager()->Render();

        sf::Sleep(5);
    }

    // Send the GoodbyeEvent to close the network connection.
    root.GetEventManager()->InjectEvent(std::make_shared<GoodbyeEvent>("The client closed the session."));
    root.GetNetworkManager()->SendQueuedEvents();

    mIsRunning = false;

    root.GetEventManager()->RemoveListener(this);
    root.Deinitialize();
}

void Game::RequestShutdown() {
    if(OnShutdownRequested())
        mIsShutdownRequested = true;
}

bool Game::OnShutdownRequested() {
    return true;
}

bool Game::IsRunning() {
    return mIsRunning;
}

} // namespace dt
