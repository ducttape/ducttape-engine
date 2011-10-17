
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef JUMPTAPE_H
#define JUMPTAPE_H

#include <Config.hpp>

#include <Core/Root.hpp>
#include <Graphics/TextComponent.hpp>
#include <Graphics/BillboardSetComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Input/InputManager.hpp>

#include <OgreBillboard.h>

#include <cstdint>

/**
  * A simple jump'N'run game.
  */
class JumpTape: public dt::State {
public:
    JumpTape();
    
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);
    
private:
    dt::Node* mGameNode;
    dt::Node* mField;
    Ogre::Billboard* mPlayer; //!< The player's avatar.
    Ogre::BillboardSet* mTiles; //!< The Set of all Tiles in the field.
    dt::TextComponent* mGameInfo; //!< Shows time and commands. 
    OIS::Keyboard* mKeyboard;
    static const uint8_t GAME_WITDH = 40;
    static const uint8_t GAME_HEIGHT = 30;
    static const uint8_t MAX_BLANK_TILE = 4; //!< Avoid too blank tiles and impossible jumps.
    static const uint8_t TILES = 15; 
    static const uint8_t PLAYER_FRAME = 12; //!< Number of frames in the player's image.
    static const float JUMP_POWER; //!< Magnitudo of the jump.
    static const float G_POWER; //!< Magnitudo of the gravity.
    static const float MAX_JUMP_HEIGHT;
    static const float TILE_X, TILE_Y; //!< Dimension of a tile.
    double mRuntime;
    uint8_t mConsecutiveBlank; //!< Consecutive blank tiles, avoid impossible jumps.

    
    /** Wheater the tile should be blank or present.
    * @return Wheater the tile should be blank (true) or present (false).
    */
    bool _GetTileType();
};

#endif // JUMPTAPE_H
