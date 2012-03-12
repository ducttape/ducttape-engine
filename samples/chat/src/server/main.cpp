
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Config.hpp>

#include "Server.hpp"

#include <Scene/Game.hpp>

#include <iostream>

int main(int argc, char** argv) {
    dt::Game game;
    Server* server = new Server();

    game.run(server, argc, argv);

    return 0;
}
