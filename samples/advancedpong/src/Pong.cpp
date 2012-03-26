#include "Pong.hpp"

#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include <Input/InputManager.hpp>
#include <Scene/StateManager.hpp>
#include <Physics/PhysicsManager.hpp>
#include <Physics/PhysicsWorld.hpp>
#include <Core/ResourceManager.hpp>
#include <Utils/Random.hpp>

#include <OgreFontManager.h>

#include <OgreProcedural.h>

float FIELD_WIDTH = 24;
float FIELD_HEIGHT = 16;
float PADDLE_SIZE = 3.0;
const float BALL_RADIUS = 0.5f;
int WINNING_SCORE = 21;
const float INITIAL_SPEED = 15.0f;
const float MAX_SPEED = 25.0f;
const float MAX_ACCELERATION = 1.0f;
const unsigned PLASMA_COUNT = 5;

void Main::resetBall() {
    if(mScore1 < WINNING_SCORE && mScore2 < WINNING_SCORE) {
        btVector3 speed(4, 0, 3);
        speed.normalize();
        speed *= INITIAL_SPEED;
        mPreviousSpeed = INITIAL_SPEED;

        mBallNode->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(speed);
    } else {
        QString p(mScore1 == WINNING_SCORE ? "left" : "right");
        getScene("testscene")->findChildNode("info")->findComponent<dt::TextComponent>("text")->setText("The " + p + " player wins the game.");
    }
	
	btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(btScalar(0),btScalar(0),btScalar(0)));

    //reset the ball position
    mBallNode->setPosition(Ogre::Vector3(0,0,0));
    mBallNode->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setWorldTransform(transform);

    //reset the paddles
    transform.setOrigin(btVector3(btScalar(- FIELD_WIDTH / 2 + 1.1f),btScalar(0),btScalar(0)));
    mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setWorldTransform(transform);
    transform.setOrigin(btVector3(btScalar(FIELD_WIDTH / 2 - 1.1f),btScalar(0),btScalar(0)));
    mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setWorldTransform(transform);

    mScore1Text->setText(dt::Utils::toString(mScore1));
    mScore2Text->setText(dt::Utils::toString(mScore2));

    mIsAtTop1 = mIsAtTop2 = mIsAtBottom1 = mIsAtBottom2 = false;
}

void Main::onInitialize() {
    dt::Random::initialize();

    mScore1 = 0;
    mScore2 = 0;

    auto scene = addScene(new dt::Scene("testscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    scene->getPhysicsWorld()->setGravity(Ogre::Vector3(0,-9.8,0));

    dt::ResourceManager::get()->addResourceLocation("","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 30, 0));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));
    camnode->findComponent<dt::CameraComponent>("cam")->getCamera();

    auto lightnode = scene->addChildNode(new dt::Node("lightnode"));
    lightnode->setPosition(Ogre::Vector3(-20, 20, 10));
    lightnode->addComponent(new dt::LightComponent("light"));

    // generate meshes
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(1.f).setSizeZ(FIELD_HEIGHT).realizeMesh("Field");
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(1.f).setSizeZ(1.f).realizeMesh("FieldWallX");
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(FIELD_HEIGHT-1).realizeMesh("FieldWallY");
    OgreProcedural::SphereGenerator().setRadius(BALL_RADIUS).realizeMesh("Ball");
    OgreProcedural::SphereGenerator().setRadius(1.0f).realizeMesh("Plasma");
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(3.f).realizeMesh("Paddle");

    mGameNode = scene->addChildNode(new dt::Node("game"));

    mFieldNode = mGameNode->addChildNode(new dt::Node("field"));
    mFieldNode->setPosition(Ogre::Vector3(0, -1.f, 0));
    mFieldNode->addComponent(new dt::MeshComponent("Field", "AdvancedPongField", "mesh"));
    mFieldNode->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 20.f));
    mFieldNode->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,0,0));
    mFieldNode->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mFieldNode->findComponent<dt::PhysicsBodyComponent>("body")->SetFriction(0.0);

    mFieldWallX1Node = mFieldNode->addChildNode(new dt::Node("fieldwallx1"));
    mFieldWallX1Node->setPosition(Ogre::Vector3(0, 1.0f, FIELD_HEIGHT / 2));
    mFieldWallX1Node->addComponent(new dt::MeshComponent("FieldWallX", "AdvancedPongField", "mesh"));
    mFieldWallX1Node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
    mFieldWallX1Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,0,0));
    mFieldWallX1Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mFieldWallX1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(0.5f);

    mFieldWallX2Node = mFieldNode->addChildNode(new dt::Node("fieldwallx2"));
    mFieldWallX2Node->setPosition(Ogre::Vector3(0, 1.0f, - FIELD_HEIGHT / 2));
    mFieldWallX2Node->addComponent(new dt::MeshComponent("FieldWallX", "AdvancedPongField", "mesh"));
    mFieldWallX2Node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
    mFieldWallX2Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,0,0));
    mFieldWallX2Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mFieldWallX2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(0.5f);

    mFieldWallY1Node = mFieldNode->addChildNode(new dt::Node("fieldwally1"));
    mFieldWallY1Node->setPosition(Ogre::Vector3(FIELD_WIDTH / 2 + 0.5, 1.0f, 0));
    mFieldWallY1Node->addComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
    mFieldWallY1Node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
    mFieldWallY1Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,0,0));
    mFieldWallY1Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mFieldWallY1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(0.5f);

    mFieldWallY2Node = mFieldNode->addChildNode(new dt::Node("fieldwally2"));
    mFieldWallY2Node->setPosition(Ogre::Vector3(- FIELD_WIDTH / 2 - 0.5, 1.0f, 0));
    mFieldWallY2Node->addComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
    mFieldWallY2Node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
    mFieldWallY2Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,0,0));
    mFieldWallY2Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mFieldWallY2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(0.5f);

    mBallNode = mGameNode->addChildNode(new dt::Node("ball"));
    mBallNode->setPosition(Ogre::Vector3(0, 1.0f, 0));
    mBallNode->addComponent(new dt::MeshComponent("Ball", "AdvancedPongBall", "mesh"));
    mBallNode->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::SPHERE, 1.f))->disableSleep(true);
    mBallNode->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(1,0,1));
    mBallNode->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,1,0));
    mBallNode->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(1.0f);
    mBallNode->findComponent<dt::PhysicsBodyComponent>("body")->SetFriction(0.0);

    for(unsigned i = 0 ; i < PLASMA_COUNT ; ++i) {
        auto plasma_node = mGameNode->addChildNode(new dt::Node(QString("plasma_") + dt::Utils::toString(i)));
        int pos_x, pos_y;
       
        pos_x = dt::Random::get(-(FIELD_WIDTH - 3) / 2, (FIELD_WIDTH - 3) / 2);
        pos_y = dt::Random::get(-(FIELD_HEIGHT - 3) / 2, (FIELD_HEIGHT - 3) / 2);

        plasma_node->setPosition(Ogre::Vector3(pos_x, 2.0f, pos_y));
        plasma_node->addComponent(new dt::MeshComponent("Plasma", "", "mesh"));
        plasma_node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::SPHERE, 0.1f));
    }

    connect(mBallNode->findComponent<dt::PhysicsBodyComponent>("body").get(), SIGNAL(collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
                                                                        this, SLOT(ballCollided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
                                                                        Qt::DirectConnection);

    mPaddle1Node = mGameNode->addChildNode(new dt::Node("paddle1"));
    mPaddle1Node->setPosition(Ogre::Vector3(- FIELD_WIDTH / 2 + 1.1f, 0.1f, 0));
    mPaddle1Node->addComponent(new dt::MeshComponent("Paddle", "AdvancedPongPaddle1", "mesh"));
    mPaddle1Node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 5.f))->disableSleep(true);
    mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,1,1));
    mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(3.0f); //for a blast off the paddle

    mPaddle2Node = mGameNode->addChildNode(new dt::Node("paddle2"));
    mPaddle2Node->setPosition(Ogre::Vector3(FIELD_WIDTH / 2 - 1.1f, 0.1f, 0));
    mPaddle2Node->addComponent(new dt::MeshComponent("Paddle", "AdvancedPongPaddle2", "mesh"));
    mPaddle2Node->addComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 5.f))->disableSleep(true);
    mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictMovement(btVector3(0,1,1));
    mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->setRestrictRotation(btVector3(0,0,0));
    mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setRestitution(3.0f); //for a blast off the paddle

    connect(mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body").get(), SIGNAL(collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
                                                                           this, SLOT(paddleCollided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)),
                                                                           Qt::DirectConnection);
    connect(mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body").get(), SIGNAL(collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
                                                                           this, SLOT(paddleCollided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
                                                                           Qt::DirectConnection);

    auto score1_node = mGameNode->addChildNode(new dt::Node("score1"));
    score1_node->setPosition(Ogre::Vector3(-10, 0, - FIELD_HEIGHT / 2 + 2));
    mScore1Text = score1_node->addComponent(new dt::TextComponent("0", "text"));
    mScore1Text->setFont("DejaVuSans");
    mScore1Text->setFontSize(64);

    auto score2_node = mGameNode->addChildNode(new dt::Node("score2"));
    score2_node->setPosition(Ogre::Vector3(10, 0, - FIELD_HEIGHT / 2 + 2));
    mScore2Text = score2_node->addComponent(new dt::TextComponent("0", "text"));
    mScore2Text->setFont("DejaVuSans");
    mScore2Text->setFontSize(64);

    auto info_node = scene->addChildNode(new dt::Node("info"));
    info_node->setPosition(Ogre::Vector3(0, 0, FIELD_HEIGHT / 2 + 3));
    auto info_text = info_node->addComponent(new dt::TextComponent("Left player: W/S -- Right player: Up/Down", "text"));
    info_text->setFont("DejaVuSans");
    info_text->setFontSize(20);

    resetBall();
}

void Main::updateStateFrame(double simulation_frame_time) {
    //mBallSpeed *= 1.0 + (simulation_frame_time * 0.05);

    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
        dt::StateManager::get()->pop();
    }

    // move paddle 1
    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_W) &&
        !mIsAtTop1) {
        //mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,-0.1f));
        mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(btVector3(0,0,-20.0f));
        if(mIsAtBottom1)
            mIsAtBottom1 = false;
    }
    else if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_S) &&
        !mIsAtBottom1) {
        //mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,0.1f));
        mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(btVector3(0,0,20.0f));
        if(mIsAtTop1)
            mIsAtTop1 = false;
    }
	else {
        mPaddle1Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(btVector3(0,0,0));
	}

    // move paddle 2
    if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_UP) &&
        !mIsAtTop2) {
        //mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,-0.1f));
        mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(btVector3(0,0,-20.0f));
        if(mIsAtBottom2)
            mIsAtBottom2 = false;
    }
    else if(dt::InputManager::get()->getKeyboard()->isKeyDown(OIS::KC_DOWN) &&
        !mIsAtBottom2) {
        //mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,0.1f));
        mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(btVector3(0,0,20.0f));
        if(mIsAtTop2)
            mIsAtTop2 = false;
    }
	else {
        mPaddle2Node->findComponent<dt::PhysicsBodyComponent>("body")->getRigidBody()->setLinearVelocity(btVector3(0,0,0));
	}

    // handle ball stuff
    auto ball = mBallNode->findComponent<dt::PhysicsBodyComponent>("body");
    auto speed = ball->getRigidBody()->getLinearVelocity();
    auto speed_value = speed.length();
    speed.normalize();

    // Acceleration control
    if (abs(speed_value - mPreviousSpeed) > MAX_ACCELERATION) {
        if (speed_value > mPreviousSpeed) {
            speed_value = mPreviousSpeed + MAX_ACCELERATION;
        }
        else {
            speed_value = mPreviousSpeed - MAX_ACCELERATION;
        }
    }

    // Speed control
    if (speed_value < 1.f) {
        speed_value = INITIAL_SPEED;
    }
    else if (speed_value > MAX_SPEED) { //keep the ball moving at a sane rate
		//ball can't travel faster than MAX_SPEED units! (per second I assume)
        speed_value = MAX_SPEED;
	}

    // set the final speed.
    mPreviousSpeed = speed_value;
    ball->getRigidBody()->setLinearVelocity(speed * speed_value);

    // Check for dropping.
    if (ball->getNode()->getPosition().y < 0.0f) {
        resetBall();
    }
}

void Main::ballCollided(dt::PhysicsBodyComponent* collider, dt::PhysicsBodyComponent* ball)
{
    if (ball->getNode()->getPosition().x >= FIELD_WIDTH / 2 - BALL_RADIUS) {
		mScore1++;
        resetBall();
	}
    else if (ball->getNode()->getPosition().x <= -FIELD_WIDTH / 2 + BALL_RADIUS) {
		mScore2++;
        resetBall();
	}
    else if (collider->getNode()->getName() == "paddle1") {
		//TODO: Graphical blast-off blow
	}
    else if (collider->getNode()->getName() == "paddle2") {
		//TODO: Graphical blast-off blow
	}
}

void Main::paddleCollided(dt::PhysicsBodyComponent* collider, dt::PhysicsBodyComponent* paddle) {
    auto name = collider->getNode()->getName();
    
    if (name == "fieldwallx1" || name == "fieldwallx2") {
        if (collider->getNode()->getName() == "fieldwallx1") {
            if(paddle->getNode()->getName() == "paddle1")
                mIsAtBottom1 = true;
            else
                mIsAtBottom2 = true;
        }
        else if(collider->getNode()->getName() == "fieldwallx2") {
            if(paddle->getNode()->getName() == "paddle1")
                mIsAtTop1 = true;
            else
                mIsAtTop2 = true;
        }
    }
}
