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

#include <OgreFontManager.h>

#include <OgreProcedural.h>

float FIELD_WIDTH = 24;
float FIELD_HEIGHT = 16;
float PADDLE_SIZE = 3.0;
int WINNING_SCORE = 21;

void Main::ResetBall() {
    if(mScore1 < WINNING_SCORE && mScore2 < WINNING_SCORE) {
		mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(20.f, 0, 0));
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
}

void Main::OnInitialize() {
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
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(1.f).realizeMesh("Ball");
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(3.f).realizeMesh("Paddle");

    mGameNode = scene->AddChildNode(new dt::Node("game"));

    mFieldNode = mGameNode->AddChildNode(new dt::Node("field"));
    mFieldNode->SetPosition(Ogre::Vector3(0, -1.f, 0));
    mFieldNode->AddComponent(new dt::MeshComponent("Field", "AdvancedPongField", "mesh"));
	mFieldNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 20.f));
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));

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
	mFieldWallY1Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2, 1.0f, 0));
	mFieldWallY1Node->AddComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
	mFieldWallY1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(0.5f);

	mFieldWallY2Node = mFieldNode->AddChildNode(new dt::Node("fieldwally2"));
	mFieldWallY2Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2, 1.0f, 0));
	mFieldWallY2Node->AddComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
	mFieldWallY2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 10.f));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(0.5f);

    mBallNode = mGameNode->AddChildNode(new dt::Node("ball"));
    mBallNode->SetPosition(Ogre::Vector3(0, 0.1f, 0));
    mBallNode->AddComponent(new dt::MeshComponent("Ball", "AdvancedPongBall", "mesh"));
	mBallNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "body", dt::PhysicsBodyComponent::BOX, 1.f))->DisableSleep(true);
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(1,1,1));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,1,0));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setRestitution(1.0f);

	connect(mBallNode->FindComponent<dt::PhysicsBodyComponent>("body"), SIGNAL(Collided(dt::PhysicsBodyComponent*)), this,
		SLOT(BallCollided(dt::PhysicsBodyComponent*)), Qt::DirectConnection);

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
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W)) {
		//mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,-0.1f));
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,-20.0f));
    }
    else if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S)) {
		//mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,0.1f));
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,20.0f));
    }
	else {
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
	}

    // move paddle 2
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_UP)) {
		//mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,-0.1f));
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,-20.0f));
    }
    else if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_DOWN)) {
		//mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0,0.1f));
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,20.0f));
    }
	else {
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
	}

    // handle ball stuff
	if (mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->getLinearVelocity().length() < 1.f || //the ball is moving too slow or is out of bounds, reset it
		mBallNode->GetPosition().x > FIELD_WIDTH / 2 || mBallNode->GetPosition().x < - FIELD_WIDTH / 2 || mBallNode->GetPosition().z > FIELD_HEIGHT / 2 || mBallNode->GetPosition().z < - FIELD_HEIGHT / 2) {
		ResetBall();
	}
	if (mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->getLinearVelocity().length() > 35.f) { //keep the ball moving at a sane rate
		//ball can't travel faster than 35 units! (per second I assume)
		mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setLinearVelocity((35 / mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->getLinearVelocity().length()) *
			mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->getLinearVelocity());
	}
}

void Main::BallCollided(dt::PhysicsBodyComponent* collider)
{
	if (collider->GetNode()->GetName() == "fieldwally1") {
		mScore1++;
		ResetBall();
	}
	else if (collider->GetNode()->GetName() == "fieldwally2") {
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
