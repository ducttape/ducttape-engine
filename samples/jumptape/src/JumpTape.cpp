
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "JumpTape.h"

#include <Core/ResourceManager.hpp>
#include <Utils/Utils.hpp>
#include <Utils/Random.hpp>
#include <Graphics/CameraComponent.hpp>

#include <OgreVector3.h>
#include <OgreProcedural.h>
#include <OgreFontManager.h>

// Initialise static const members 
const float JumpTape::JUMP_POWER = 0.01;
const float JumpTape::G_POWER = 0.005;
const float JumpTape::MAX_JUMP_HEIGHT = 12;
const float JumpTape::TILE_X = 4;
const float JumpTape::TILE_Y = 0.6;

JumpTape::JumpTape() : 
    mGameNode(nullptr),
    mRuntime(0),
    mConsecutiveBlank(0) {}

void JumpTape::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    mKeyboard = dt::InputManager::Get()->GetKeyboard();
    double field_speed = mRuntime/1000; 
    
    // Move each tile. 
    for(uint8_t i=0; i<TILES; ++i) {
        Ogre::Billboard* tile = mTiles->getBillboard(i);
        tile->setPosition(tile->getPosition() - Ogre::Vector3(field_speed, 0, 0));
    }
    
    // Wheater the next tile should be blank or present.
    bool blank = _GetTileType();
    
    // Move the first tile at the end of the group if it went beyond left margin.
    static uint8_t i = 0; //  index for selecting the right tile.
    Ogre::Billboard* tile = mTiles->getBillboard(i);
    if(tile->getPosition().x < (-GAME_WITDH/2)-4) {
        // Get position of the last (precedent) tile.
        double last_tile_x = mTiles->getBillboard((i+TILES-1) % TILES)->getPosition().x;
        // Append billboard to end.
        tile->setPosition(last_tile_x+TILE_X, dt::Random::Get(-3, 3), 0); 
        tile->setTexcoordIndex(blank);
        i++; // Switch to next tile...
        i %= TILES; // ...in a cicle.
    }
    
    // Move player.
    // Player and tile variables.
    tile = mTiles->getBillboard((i+2) % TILES); // The tile on which the player is.
    static float jump_height = 0; // The height of each jump.
    static bool jump_allowed = false; // Jump is not allowed when the player is already on air.
    bool blank_tile = tile->getTexcoordIndex(); // Wheater the tile under the player is blank.
    Ogre::Vector3 player_pos = mPlayer->getPosition();
    Ogre::Vector3 tile_pos = tile->getPosition(); // Position of the tile under the player.
    
    // Jump.
    if(mKeyboard->isKeyDown(OIS::KC_SPACE) 
        && (jump_height < MAX_JUMP_HEIGHT) && jump_allowed) {
        player_pos += Ogre::Vector3(0, JUMP_POWER, 0);
        mPlayer->setPosition(player_pos);
        jump_height += JUMP_POWER;
    }
    else {
        jump_allowed = false; // Once the player release space he can't go up anymore.
    }
    
    
    float tile_top = tile_pos.y + TILE_Y * 2; // Top of the tile.
    // Wheater the player is running on the tile.
    bool on_tile = (player_pos.y <= tile_top+0.1) && (player_pos.y >= tile_top-0.1); 
    
    // Gravity.
    if((blank_tile || !on_tile) && !jump_allowed) {
        player_pos -= Ogre::Vector3(0, G_POWER, 0);
        mPlayer->setPosition(player_pos);
    }
    
    // Evaluate if jump is allowed.
    if(on_tile && !blank_tile) {
        jump_height = 0;
        jump_allowed = true;
        // The player is running, change animation.
        mPlayer->setTexcoordIndex(static_cast<uint8_t>(mRuntime*10) % PLAYER_FRAME);
    }
    
    // Reset game.
    if(mKeyboard->isKeyDown(OIS::KC_N)) {
        mRuntime=0;
        mPlayer->setPosition(mPlayer->getPosition().x, tile_pos.y+4, 0);
        tile->setTexcoordIndex(0);
    }
    
    // Show reset game info.
    static QString time;
    if(player_pos.y < -4) {
        mGameInfo->SetText("You ran for " + time + " seconds, press N for a new game");
    }
    else {
        time = dt::Utils::ToString(static_cast<uint32_t>(mRuntime));
        mGameInfo->SetText("You ran for " + time + " seconds");
    }
}

void JumpTape::OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("JumpTape_scene"));
        OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

        dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->SetPosition(Ogre::Vector3(0, 0, 40));
        camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));
        
        dt::Node* background = scene->AddChildNode(new dt::Node("background"));
        background->SetPosition(Ogre::Vector3(0, 0, -1));
        dt::BillboardSetComponent* background_billboard = background->AddComponent(
            new dt::BillboardSetComponent(
                "background_billboard", 1, "jumptape-background.jpg"));
        background_billboard->GetOgreBillboardSet()->setDefaultDimensions(50, 34);

        mField = scene->AddChildNode(new dt::Node("field_node"));
        mField->SetPosition(Ogre::Vector3(0, 0, 0));
        
        dt::BillboardSetComponent* billboard = mField->AddComponent(
            new dt::BillboardSetComponent("tiles", TILES));
        billboard->SetTextureFromFile("jumptape-tiles.png");
        mTiles = billboard->GetOgreBillboardSet();
        mTiles->setTextureStacksAndSlices(1, 2);
        mTiles->setDefaultDimensions(TILE_X, TILE_Y);
        mTiles->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY); // Always visible
        
        for(uint8_t i=0; i<TILES; ++i) {    
            // Consecutive blank tiles, must be under MAX_BLANK_TILE,
            // otherwise the player can't reach the other tile.
            bool blank; 
            
            if(i<5) { // Plain start.
               blank = false;
            }
            else {
               blank = _GetTileType();
            }
            
            mTiles->setBillboardOrigin(Ogre::BBO_CENTER);
            Ogre::Billboard* tile = mTiles->createBillboard((-GAME_WITDH/2)+(TILE_X*i), 0, 0);
            tile->setTexcoordIndex(blank);
        }

        dt::Node* player = scene->AddChildNode(new dt::Node("player"));
        player->SetPosition(Ogre::Vector3(0, 0, 0));
        dt::BillboardSetComponent* billboard_component = 
            player->AddComponent(new dt::BillboardSetComponent("player", 
                                                               1, "jumptape-jumper.png"));
        Ogre::BillboardSet* player_billboard = billboard_component->GetOgreBillboardSet(); 
        player_billboard->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
        player_billboard->setTextureStacksAndSlices(1, 12);
        player_billboard->setDefaultDimensions(2, 2);
        mPlayer = player_billboard->getBillboard(0);
        mPlayer->setPosition(mTiles->getBillboard(2)->getPosition().x-1.6, // Match edges of image.
                             mTiles->getBillboard(2)->getPosition().y+9, 0);
        
        dt::Node* info_node = scene->AddChildNode(new dt::Node("info"));
        info_node->SetPosition(Ogre::Vector3(0, (-GAME_HEIGHT/2)+5, 2));
        mGameInfo = info_node->AddComponent(new dt::TextComponent(""));
        mGameInfo->SetFont("DejaVuSans");
        mGameInfo->SetFontSize(20);
        mGameInfo->SetColor(Ogre::ColourValue::White);
}

bool JumpTape::_GetTileType() {
     bool blank;
     if(mConsecutiveBlank >= MAX_BLANK_TILE) { // Avoid too blank tiles and impossible jumps.
               blank = false;
               mConsecutiveBlank =- 1;
     } 
     else {
        blank = static_cast<uint8_t>(dt::Random::Get(0, 1));
     }
     return blank;
}

