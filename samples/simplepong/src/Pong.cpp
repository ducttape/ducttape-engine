#include "Pong.hpp"

#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include <Input/InputManager.hpp>
#include <Core/ResourceManager.hpp>

#include <OgreFontManager.h>

#include <OgreProcedural.h>

float FIELD_WIDTH = 24;
float FIELD_HEIGHT = 16;
float PADDLE_SIZE = 3.0;

void Main::resetBall() {
    if(mScore1 < 10 && mScore2 < 10) {
        mBallSpeed = Ogre::Vector3(4, -3, 0);
    } else {
        mBallSpeed = Ogre::Vector3::ZERO;
        QString p(mScore1 == 10 ? "left" : "right");
        getScene("testscene")->findChildNode("info")->findComponent<dt::TextComponent>("text")->setText("The " + p + " player wins the game.");
    }
    mBallNode->setPosition(Ogre::Vector3(0,0,0));
    mScore1Text->setText(dt::Utils::toString(mScore1));
    mScore2Text->setText(dt::Utils::toString(mScore2));
}

void Main::onInitialize() {
    mScore1 = 0;
    mScore2 = 0;

    auto scene = addScene(new dt::Scene("testscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    dt::ResourceManager::get()->addResourceLocation("","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, 30));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    auto lightnode = scene->addChildNode(new dt::Node("lightnode"));
    lightnode->setPosition(Ogre::Vector3(-20, 20, 10));
    lightnode->addComponent(new dt::LightComponent("light"));

    // generate meshes
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(FIELD_HEIGHT).setSizeZ(1.f).realizeMesh("Field");
    OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(1.f).setSizeZ(1.f).realizeMesh("Ball");
    OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(3.f).setSizeZ(1.f).realizeMesh("Paddle");

    mGameNode = scene->addChildNode(new dt::Node("game"));

    mFieldNode = mGameNode->addChildNode(new dt::Node("field"));
    mFieldNode->setPosition(Ogre::Vector3(0, 0, -1));
    mFieldNode->addComponent(new dt::MeshComponent("Field", "SimplePongField", "mesh"));

    mBallNode = mGameNode->addChildNode(new dt::Node("ball"));
    mBallNode->setPosition(Ogre::Vector3(0, 0, 0));
    mBallNode->addComponent(new dt::MeshComponent("Ball", "SimplePongBall", "mesh"));

    mPaddle1Node = mGameNode->addChildNode(new dt::Node("paddle1"));
    mPaddle1Node->setPosition(Ogre::Vector3(- FIELD_WIDTH / 2 - 0.5, 0, 0));
    mPaddle1Node->addComponent(new dt::MeshComponent("Paddle", "SimplePongPaddle", "mesh"));

    mPaddle2Node = mGameNode->addChildNode(new dt::Node("paddle2"));
    mPaddle2Node->setPosition(Ogre::Vector3(FIELD_WIDTH / 2 + 0.5, 0, 0));
    mPaddle2Node->addComponent(new dt::MeshComponent("Paddle", "SimplePongPaddle", "mesh"));

    auto score1_node = mGameNode->addChildNode(new dt::Node("score1"));
    score1_node->setPosition(Ogre::Vector3(-10, FIELD_HEIGHT / 2 + 2, 0));
    mScore1Text = score1_node->addComponent(new dt::TextComponent("0", "text"));
    mScore1Text->setFont("DejaVuSans");
    mScore1Text->setFontSize(64);

    auto score2_node = mGameNode->addChildNode(new dt::Node("score2"));
    score2_node->setPosition(Ogre::Vector3(10, FIELD_HEIGHT / 2 + 2, 0));
    mScore2Text = score2_node->addComponent(new dt::TextComponent("0", "text"));
    mScore2Text->setFont("DejaVuSans");
    mScore2Text->setFontSize(64);

    auto info_node = scene->addChildNode(new dt::Node("info"));
    info_node->setPosition(Ogre::Vector3(0, - FIELD_HEIGHT / 2 - 3, 0));
    auto info_text = info_node->addComponent(new dt::TextComponent("Left player: W/S -- Right player: Up/Down", "text"));
    info_text->setFont("DejaVuSans");
    info_text->setFontSize(20);

    resetBall();
}

void Main::updateStateFrame(double simulation_frame_time) {
    mBallSpeed *= 1.0 + (simulation_frame_time * 0.05);

    // move paddle 1
    float move1 = 0;
    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_W)) {
        move1 += 1;
    }
    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_S)) {
        move1 -= 1;
    }
    float new_y1 = mPaddle1Node->getPosition().y + move1 * simulation_frame_time * 8;
    if(new_y1 > FIELD_HEIGHT / 2 - PADDLE_SIZE / 2)
        new_y1 = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
    else if(new_y1 < - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2)
        new_y1 = - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2;

    mPaddle1Node->setPosition(Ogre::Vector3(
                mPaddle1Node->getPosition().x,
                new_y1,
                mPaddle1Node->getPosition().z));

    // move paddle 2
    float move2 = 0;
    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_UP)) {
        move2 += 1;
    }

    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_DOWN)) {
        move2 -= 1;
    }

    float new_y2 = mPaddle2Node->getPosition().y + move2 * simulation_frame_time * 8;
    if(new_y2 > FIELD_HEIGHT / 2 - PADDLE_SIZE / 2)
        new_y2 = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
    else if(new_y2 < - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2)
        new_y2 = - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2;

    mPaddle2Node->setPosition(Ogre::Vector3(
                mPaddle2Node->getPosition().x,
                new_y2,
                mPaddle2Node->getPosition().z));

    // move ball
    Ogre::Vector3 newpos(mBallNode->getPosition() + mBallSpeed * simulation_frame_time);
    if(newpos.y >= FIELD_HEIGHT / 2 - 0.5 || newpos.y <= -FIELD_HEIGHT / 2 + 0.5) {
        mBallSpeed.y *= -1;
    }

    if(newpos.x >= FIELD_WIDTH / 2 - 0.5) {
        float paddle_y = mPaddle2Node->getPosition().y;
        if(newpos.y < paddle_y - PADDLE_SIZE / 2 - 0.5 || newpos.y > paddle_y + PADDLE_SIZE / 2 + 0.5) {
            dt::Logger::get().info("Player lost!");
            ++mScore1;
            resetBall();
        } else {
            mBallSpeed.x *= -1;
        }
    } else if(newpos.x <= -FIELD_WIDTH / 2 + 0.5) {
        float paddle_y = mPaddle1Node->getPosition().y;
        if(newpos.y < paddle_y - PADDLE_SIZE / 2 - 0.5 || newpos.y > paddle_y + PADDLE_SIZE / 2 + 0.5) {
            dt::Logger::get().info("Computer lost!");
            ++mScore2;
            resetBall();
        } else {
            mBallSpeed.x *= -1;
        }
    }

    mBallNode->setPosition(mBallNode->getPosition() + mBallSpeed * simulation_frame_time);

    Ogre::Quaternion q;
    q.FromAngleAxis(Ogre::Radian(Ogre::Math::Cos(Ogre::Radian(dt::Root::getInstance().getTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_X);
    Ogre::Quaternion w;
    w.FromAngleAxis(Ogre::Radian(Ogre::Math::Sin(Ogre::Radian(dt::Root::getInstance().getTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_Y);
    mGameNode->setRotation(q * w);
}
