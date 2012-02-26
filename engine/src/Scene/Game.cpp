
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Game.hpp>

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Input/InputManager.hpp>
#include <Network/NetworkManager.hpp>
#include <Network/GoodbyeEvent.hpp>
#include <Graphics/DisplayManager.hpp>

#include <SFML/System/Sleep.hpp>
#include <SFML/Audio/Listener.hpp>

namespace dt {

Game::Game()
    : mIsShutdownRequested(false),
      mIsRunning(false) {}

void Game::Run(State* start_state, int argc, char** argv) {
    Root& root = Root::GetInstance();

    root.Initialize(argc, argv);
    root.GetStateManager()->SetNewState(start_state);
    QObject::connect(root.GetInputManager(), SIGNAL(WindowClosed()), this, SLOT(RequestShutdown()));
    //connect BeginFrames to things that need it, like State/Scenes and the PhysicsManager
    QObject::connect(this, SIGNAL(BeginFrame(double)), root.GetStateManager(), SIGNAL(BeginFrame(double)), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(BeginFrame(double)), (QObject*)root.GetPhysicsManager(),
            SLOT(UpdateFrame(double)), Qt::DirectConnection);

    mClock.Restart();
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
        double frame_time = mClock.GetElapsedTime().AsSeconds();
        mClock.Restart();

        // Shift states and cancel if none are left
        if(!root.GetStateManager()->ShiftStates())
            break;

        // INPUT
        InputManager::Get()->Capture();

        accumulator += frame_time;
        while(accumulator >= simulation_frame_time) {
            anti_spiral_clock.Restart();
            // SIMULATION
            emit BeginFrame(simulation_frame_time);


            // NETWORKING
            root.GetNetworkManager()->SendQueuedEvents();

            double real_simulation_time = anti_spiral_clock.GetElapsedTime().AsSeconds();
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

        // Update the listener.
        auto main_camera = root.GetDisplayManager()->GetMainCamera();
        if(main_camera != nullptr) {
            auto pos = main_camera->GetNode()->GetPosition();
            auto dir = main_camera->GetCamera()->getDirection();
            sf::Listener::SetPosition(pos.x, pos.y, pos.z);
            sf::Listener::SetDirection(dir.x, dir.y, dir.z);
        }

        sf::Sleep(sf::Milliseconds(5));
    }

    // Send the GoodbyeEvent to close the network connection.
    root.GetNetworkManager()->QueueEvent(std::make_shared<GoodbyeEvent>("The client closed the session."));
    root.GetNetworkManager()->SendQueuedEvents();

    mIsRunning = false;

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
