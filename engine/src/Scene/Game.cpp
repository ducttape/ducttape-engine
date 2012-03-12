
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

void Game::run(State* start_state, int argc, char** argv) {
    Root& root = Root::getInstance();

    root.initialize(argc, argv);
    root.getStateManager()->setNewState(start_state);
    QObject::connect(root.getInputManager(), SIGNAL(windowClosed()),
                     this,                   SLOT(requestShutdown()));
    //connect BeginFrames to things that need it, like State/Scenes and the PhysicsManager
    QObject::connect(this,                   SIGNAL(beginFrame(double)),
                     root.getStateManager(), SIGNAL(beginFrame(double)), Qt::DirectConnection);
    QObject::connect(this,                               SIGNAL(beginFrame(double)),
                     (QObject*)root.getPhysicsManager(), SLOT(updateFrame(double)), Qt::DirectConnection);

    mClock.restart();
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
        double frame_time = mClock.getElapsedTime().asSeconds();
        mClock.restart();

        // Shift states and cancel if none are left
        if(!root.getStateManager()->shiftStates())
            break;

        // INPUT
        InputManager::get()->capture();

        accumulator += frame_time;
        while(accumulator >= simulation_frame_time) {
            anti_spiral_clock.restart();
            // SIMULATION
            emit beginFrame(simulation_frame_time);


            // NETWORKING
            root.getNetworkManager()->sendQueuedEvents();

            double real_simulation_time = anti_spiral_clock.getElapsedTime().asSeconds();
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
        root.getDisplayManager()->render();

        // Update the listener.
        auto main_camera = root.getDisplayManager()->getMainCamera();
        if(main_camera != nullptr) {
            auto pos = main_camera->getNode()->getPosition();
            auto dir = main_camera->getCamera()->getDirection();
            sf::Listener::setPosition(pos.x, pos.y, pos.z);
            sf::Listener::setDirection(dir.x, dir.y, dir.z);
        }

        sf::sleep(sf::milliseconds(5));
    }

    // Send the GoodbyeEvent to close the network connection.
    root.getNetworkManager()->queueEvent(std::make_shared<GoodbyeEvent>("The client closed the session."));
    root.getNetworkManager()->sendQueuedEvents();

    mIsRunning = false;

    root.deinitialize();
}

void Game::requestShutdown() {
    if(onShutdownRequested())
        mIsShutdownRequested = true;
}

bool Game::onShutdownRequested() {
    return true;
}

bool Game::isRunning() {
    return mIsRunning;
}

} // namespace dt
