
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

class JumpTape: public dt::State {
public:
    JumpTape();
    
    void HandleEvent(std::shared_ptr< dt::Event > e);
    void OnInitialize();
private:
    dt::Node* mGameNode;
    dt::Node* mField;
    Ogre::Billboard* mPlayer; //!< The player avatar.
    Ogre::BillboardSet* mTiles; //!< The Set of all Tiles of the field.
    dt::TextComponent* mGameInfo;
    OIS::Keyboard* mKeyboard;
    static const uint8_t GAME_WITDH = 40;
    static const uint8_t GAME_HEIGHT = 30;
    static const uint8_t MAX_BLANK_TILE = 4; //!< Avoid too blank tiles and impossible jump.
    static const uint8_t TILES = 15; 
    static const uint8_t PLAYER_FRAME = 12; //!< Number of frame in the player image.
    static const float JUMP_POWER; //!< Magnitudo of the jump.
    static const float MAX_JUMP_HEIGHT;
    static const float TILE_X, TILE_Y; //!< Dimension of a tile.
    double mRuntime;
    double mFieldSpeed;
    uint8_t mConsecutiveBlank; //!< Consecutive blank tiles, necessary to avoid impossible jump.

    
    /** Wheater the tile should be blank or present.
    * @return Wheater the tile should be blank (true) or present (false).
    */
    bool _GetTileType();
};

#endif // JUMPTAPE_H
