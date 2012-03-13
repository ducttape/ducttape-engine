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

void Main::ResetBall() {
    if(mScore1 < WINNING_SCORE && mScore2 < WINNING_SCORE) {
        btVector3 speed(4, 0, 3);
        speed.normalize();
        speed *= INITIAL_SPEED;
        mPreviousSpeed = INITIAL_SPEED;

		mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(speed);
    } else {
        QString p(mScore1 == WINNING_SCORE ? "left" : "right");
        GetScene("testscene")->FindChildNode("info")->FindComponent<dt::TextComponent>("text")->SetText("The " + p + " player wins the game.");
    }
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(btScalar(0),btScalar(0),btScalar(0)));

	//reset the ball position
    mBallNode->SetPosition(Ogre::Vector3(0,0,0));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setWorldTransform(transform);

	//reset the paddles
	transform.setOrigin(btVector3(btScalar(- FIELD_WIDTH / 2 + 1.1f),btScalar(0),btScalar(0)));
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setWorldTransform(transform);
	transform.setOrigin(btVector3(btScalar(FIELD_WIDTH / 2 - 1.1f),btScalar(0),btScalar(0)));
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setWorldTransform(transform);

    mScore1Text->SetText(dt::Utils::ToString(mScore1));
    mScore2Text->SetText(dt::Utils::ToString(mScore2));

    mIsAtTop1 = mIsAtTop2 = mIsAtBottom1 = mIsAtBottom2 = false;
}

void Main::OnInitialize() {
    dt::Random::Initialize();

    mScore1 = 0;
    mScore2 = 0;

    dt::Scene* scene = AddScene(new dt::Scene("testscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

	scene->GetPhysicsWorld()->SetGravity(Ogre::Vector3(0,-9.8,0));

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 30, 0));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));
	camnode->FindComponent<dt::CameraComponent>("cam")->GetCamera();

    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    lightnode->SetPosition(Ogre::Vector3(-20, 20, 10));
    lightnode->AddComponent(new dt::LightComponent("light"));

    // generate meshes
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(1.f).setSizeZ(FIELD_HEIGHT).realizeMesh("Field");
	OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(1.f).setSizeZ(1.f).realizeMesh("FieldWallX");
	OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(FIELD_HEIGHT-1).realizeMesh("FieldWallY");
    OgreProcedural::SphereGenerator().setRadius(BALL_RADIUS).realizeMesh("Ball");
    OgreProcedural::SphereGenerator().setRadius(1.0f).realizeMesh("Plasma");
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(3.f).realizeMesh("Paddle");

    mGameNode = scene->AddChildNode(new dt::Node("game"));

    mFieldNode = mGameNode->AddChildNode(new dt::Node("field"));
    mFieldNode->SetPosition(Ogre::Vector3(0, -1.f, 0));
    mFieldNode->AddComponent(new dt::MeshComponent("Field", "AdvancedPongField", "mesh"));
	mFieldNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 20.f));
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
    mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetFriction(0.0);

	mFieldWallX1Node = mFieldNode->AddChildNode(new dt::Node("fieldwallx1"));
	mFieldWallX1Node->SetPosition(Ogre::Vector3(0, 1.0f, FIELD_HEIGHT / 2));
	mFieldWallX1Node->AddComponent(new dt::MeshComponent("FieldWallX", "AdvancedPongField", "mesh"));
	mFieldWallX1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
	mFieldWallX1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallX1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallX1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(0.5f);

	mFieldWallX2Node = mFieldNode->AddChildNode(new dt::Node("fieldwallx2"));
	mFieldWallX2Node->SetPosition(Ogre::Vector3(0, 1.0f, - FIELD_HEIGHT / 2));
	mFieldWallX2Node->AddComponent(new dt::MeshComponent("FieldWallX", "AdvancedPongField", "mesh"));
	mFieldWallX2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
	mFieldWallX2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallX2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallX2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(0.5f);

	mFieldWallY1Node = mFieldNode->AddChildNode(new dt::Node("fieldwally1"));
	mFieldWallY1Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2 + 0.5, 1.0f, 0));
	mFieldWallY1Node->AddComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
	mFieldWallY1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(0.5f);

	mFieldWallY2Node = mFieldNode->AddChildNode(new dt::Node("fieldwally2"));
	mFieldWallY2Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2 - 0.5, 1.0f, 0));
	mFieldWallY2Node->AddComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
	mFieldWallY2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(0.5f);

    mBallNode = mGameNode->AddChildNode(new dt::Node("ball"));
    mBallNode->SetPosition(Ogre::Vector3(0, 1.0f, 0));
    mBallNode->AddComponent(new dt::MeshComponent("Ball", "AdvancedPongBall", "mesh"));
    mBallNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::SPHERE, 1.f))->DisableSleep(true);
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(1,0,1));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,1,0));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(1.0f);
    mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetFriction(0.0);

    for(unsigned i = 0 ; i < PLASMA_COUNT ; ++i) {
        dt::Node* plasma_node = mGameNode->AddChildNode(new dt::Node(QString("plasma_") + dt::Utils::ToString(i)));
        int pos_x, pos_y;
       
        pos_x = dt::Random::Get(-(FIELD_WIDTH - 3) / 2, (FIELD_WIDTH - 3) / 2);
        pos_y = dt::Random::Get(-(FIELD_HEIGHT - 3) / 2, (FIELD_HEIGHT - 3) / 2);

        plasma_node->SetPosition(Ogre::Vector3(pos_x, 2.0f, pos_y));
        plasma_node->AddComponent(new dt::MeshComponent("Plasma", "", "mesh"));
        plasma_node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::SPHERE, 0.1f));
    }

	connect(mBallNode->FindComponent<dt::PhysicsBodyComponent>("body"), SIGNAL(Collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), this,
		SLOT(BallCollided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection);

    mPaddle1Node = mGameNode->AddChildNode(new dt::Node("paddle1"));
    mPaddle1Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2 + 1.1f, 0.1f, 0));
    mPaddle1Node->AddComponent(new dt::MeshComponent("Paddle", "AdvancedPongPaddle1", "mesh"));
	mPaddle1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 5.f))->DisableSleep(true);
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,1,1));
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(3.0f); //for a blast off the paddle

    mPaddle2Node = mGameNode->AddChildNode(new dt::Node("paddle2"));
    mPaddle2Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2 - 1.1f, 0.1f, 0));
    mPaddle2Node->AddComponent(new dt::MeshComponent("Paddle", "AdvancedPongPaddle2", "mesh"));
	mPaddle2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 5.f))->DisableSleep(true);
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,1,1));
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(3.0f); //for a blast off the paddle

    connect(mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body"), SIGNAL(Collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), this,
		SLOT(PaddleCollided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection);
    connect(mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body"), SIGNAL(Collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), this,
		SLOT(PaddleCollided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection);

    dt::Node* score1_node = mGameNode->AddChildNode(new dt::Node("score1"));
    score1_node->SetPosition(Ogre::Vector3(-10, 0, - FIELD_HEIGHT / 2 + 2));
    mScore1Text = score1_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore1Text->SetFont("DejaVuSans");
    mScore1Text->SetFontSize(64);

    dt::Node* score2_node = mGameNode->AddChildNode(new dt::Node("score2"));
    score2_node->SetPosition(Ogre::Vector3(10, 0, - FIELD_HEIGHT / 2 + 2));
    mScore2Text = score2_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore2Text->SetFont("DejaVuSans");
    mScore2Text->SetFontSize(64);

    dt::Node* info_node = scene->AddChildNode(new dt::Node("info"));
    info_node->SetPosition(Ogre::Vector3(0, 0, FIELD_HEIGHT / 2 + 3));
    dt::TextComponent* info_text = info_node->AddComponent(new dt::TextComponent("Left player: W/S -- Right player: Up/Down", "text"));
    info_text->SetFont("DejaVuSans");
    info_text->SetFontSize(20);

    ResetBall();
}

void Main::UpdateStateFrame(double simulation_frame_time) {
    //mBallSpeed *= 1.0 + (simulation_frame_time * 0.05);

	if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
        dt::StateManager::Get()->Pop();
    }

    // move paddle 1
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W) &&
        !mIsAtTop1) {
		//mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,-0.1f));
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,-20.0f));
        if(mIsAtBottom1)
            mIsAtBottom1 = false;
    }
    else if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S) &&
        !mIsAtBottom1) {
		//mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,0.1f));
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,20.0f));
        if(mIsAtTop1)
            mIsAtTop1 = false;
    }
	else {
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
	}

    // move paddle 2
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_UP) &&
        !mIsAtTop2) {
		//mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,-0.1f));
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,-20.0f));
        if(mIsAtBottom2)
            mIsAtBottom2 = false;
    }
    else if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_DOWN) &&
        !mIsAtBottom2) {
		//mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,0.1f));
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,20.0f));
        if(mIsAtTop2)
            mIsAtTop2 = false;
    }
	else {
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
	}

    // handle ball stuff
    dt::PhysicsBodyComponent* ball = mBallNode->FindComponent<dt::PhysicsBodyComponent>("body");
    auto speed = ball->GetRigidBody()->getLinearVelocity();
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

    // Set the final speed.
    mPreviousSpeed = speed_value;
    ball->GetRigidBody()->setLinearVelocity(speed * speed_value);

    // Check for dropping.
    if (ball->GetNode()->GetPosition().y < 0.0f) {
        ResetBall();
    }
}

void Main::BallCollided(dt::PhysicsBodyComponent* collider, dt::PhysicsBodyComponent* ball)
{
    if (ball->GetNode()->GetPosition().x >= FIELD_WIDTH / 2 - BALL_RADIUS) {
		mScore1++;
		ResetBall();
	}
	else if (ball->GetNode()->GetPosition().x <= -FIELD_WIDTH / 2 + BALL_RADIUS) {
		mScore2++;
		ResetBall();
	}
	else if (collider->GetNode()->GetName() == "paddle1") {
		//TODO: Graphical blast-off blow
	}
	else if (collider->GetNode()->GetName() == "paddle2") {
		//TODO: Graphical blast-off blow
	}
}

void Main::PaddleCollided(dt::PhysicsBodyComponent* collider, dt::PhysicsBodyComponent* paddle) {
    auto name = collider->GetNode()->GetName();
    
    if (name == "fieldwallx1" || name == "fieldwallx2") {
        if (collider->GetNode()->GetName() == "fieldwallx1") {
            if(paddle->GetNode()->GetName() == "paddle1")
                mIsAtBottom1 = true;
            else
                mIsAtBottom2 = true;
        }
        else if(collider->GetNode()->GetName() == "fieldwallx2") {
            if(paddle->GetNode()->GetName() == "paddle1")
                mIsAtTop1 = true;
            else
                mIsAtTop2 = true;
        }
    }
}