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

void Main::ResetBall() {
    if(mScore1 < 10 && mScore2 < 10) {
        mBallSpeed = Ogre::Vector3(-0.1f, 0, 0);
    } else {
        mBallSpeed = Ogre::Vector3::ZERO;
        QString p(mScore1 == 10 ? "left" : "right");
        GetScene("testscene")->FindChildNode("info")->FindComponent<dt::TextComponent>("text")->SetText("The " + p + " player wins the game.");
    }
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(btScalar(0),btScalar(0),btScalar(0.1f)));

    mBallNode->SetPosition(Ogre::Vector3(0,0,0.1f));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->GetRigidBody()->setWorldTransform(transform);
    mScore1Text->SetText(dt::Utils::ToString(mScore1));
    mScore2Text->SetText(dt::Utils::ToString(mScore2));
}

void Main::OnInitialize() {
    mScore1 = 0;
    mScore2 = 0;

    dt::Scene* scene = AddScene(new dt::Scene("testscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

	scene->GetPhysicsWorld()->SetGravity(Ogre::Vector3(0,0,-9.8));

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 0, 30));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));
	camnode->FindComponent<dt::CameraComponent>("cam")->GetCamera();

    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    lightnode->SetPosition(Ogre::Vector3(-20, 20, 10));
    lightnode->AddComponent(new dt::LightComponent("light"));

    // generate meshes
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(FIELD_HEIGHT).setSizeZ(1.f).realizeMesh("Field");
	OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(1.f).setSizeZ(1.f).realizeMesh("FieldWallX");
	OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(FIELD_HEIGHT-1).setSizeZ(1.f).realizeMesh("FieldWallY");
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(1.f).setSizeZ(1.f).realizeMesh("Ball");
    OgreProcedural::BoxGenerator().setSizeX(1.f).setSizeY(3.f).setSizeZ(1.f).realizeMesh("Paddle");

    mGameNode = scene->AddChildNode(new dt::Node("game"));

    mFieldNode = mGameNode->AddChildNode(new dt::Node("field"));
    mFieldNode->SetPosition(Ogre::Vector3(0, 0, -1));
    mFieldNode->AddComponent(new dt::MeshComponent("Field", "AdvancedPongField", "mesh"));
	mFieldNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->SetMass(20.f);
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

	mFieldWallX1Node = mFieldNode->AddChildNode(new dt::Node("fieldwallx1"));
	mFieldWallX1Node->SetPosition(Ogre::Vector3(0, FIELD_HEIGHT / 2, 1.0f));
	mFieldWallX1Node->AddComponent(new dt::MeshComponent("FieldWallX", "AdvancedPongField", "mesh"));
	mFieldWallX1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->SetMass(10.f);
	mFieldWallX1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallX1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallX1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

	mFieldWallX2Node = mFieldNode->AddChildNode(new dt::Node("fieldwallx2"));
	mFieldWallX2Node->SetPosition(Ogre::Vector3(0,- FIELD_HEIGHT / 2, 1.0f));
	mFieldWallX2Node->AddComponent(new dt::MeshComponent("FieldWallX", "AdvancedPongField", "mesh"));
	mFieldWallX2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->SetMass(10.f);
	mFieldWallX2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallX2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallX2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

	mFieldWallY1Node = mFieldNode->AddChildNode(new dt::Node("fieldwally1"));
	mFieldWallY1Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2, 0, 1.0f));
	mFieldWallY1Node->AddComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
	mFieldWallY1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->SetMass(10.f);
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallY1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

	mFieldWallY2Node = mFieldNode->AddChildNode(new dt::Node("fieldwally2"));
	mFieldWallY2Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2, 0, 1.0f));
	mFieldWallY2Node->AddComponent(new dt::MeshComponent("FieldWallY", "AdvancedPongField", "mesh"));
	mFieldWallY2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->SetMass(10.f);
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,0,0));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mFieldWallY2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

    mBallNode = mGameNode->AddChildNode(new dt::Node("ball"));
    mBallNode->SetPosition(Ogre::Vector3(0, 0, 0.1f));
    mBallNode->AddComponent(new dt::MeshComponent("Ball", "AdvancedPongBall", "mesh"));
	mBallNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->DisableSleep(true);
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(1,1,0));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,1));
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);
	connect(mBallNode->FindComponent<dt::PhysicsBodyComponent>("body"), SIGNAL(Collided(dt::PhysicsBodyComponent*)), this,
		SLOT(BallCollided(dt::PhysicsBodyComponent*)), Qt::DirectConnection);

    mPaddle1Node = mGameNode->AddChildNode(new dt::Node("paddle1"));
    mPaddle1Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2 + 1.1f, 0, 5.f));
    mPaddle1Node->AddComponent(new dt::MeshComponent("Paddle", "AdvancedPongPaddle1", "mesh"));
	mPaddle1Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"))->DisableSleep(true);
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,1,1));
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mPaddle1Node->FindComponent<dt::MeshComponent>("mesh")->GetOgreEntity()->getMesh();
	mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

    mPaddle2Node = mGameNode->AddChildNode(new dt::Node("paddle2"));
    mPaddle2Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2 - 1.1f, 0, 5.f));
    mPaddle2Node->AddComponent(new dt::MeshComponent("Paddle", "AdvancedPongPaddle2", "mesh"));
	mPaddle2Node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"));
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictMovement(btVector3(0,1,1));
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetRestrictRotation(btVector3(0,0,0));
	mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->SetCollisionShapeType(dt::PhysicsBodyComponent::BOX);

    dt::Node* score1_node = mGameNode->AddChildNode(new dt::Node("score1"));
    score1_node->SetPosition(Ogre::Vector3(-10, FIELD_HEIGHT / 2 + 2, 0));
    mScore1Text = score1_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore1Text->SetFont("DejaVuSans");
    mScore1Text->SetFontSize(64);

    dt::Node* score2_node = mGameNode->AddChildNode(new dt::Node("score2"));
    score2_node->SetPosition(Ogre::Vector3(10, FIELD_HEIGHT / 2 + 2, 0));
    mScore2Text = score2_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore2Text->SetFont("DejaVuSans");
    mScore2Text->SetFontSize(64);

    dt::Node* info_node = scene->AddChildNode(new dt::Node("info"));
    info_node->SetPosition(Ogre::Vector3(0, - FIELD_HEIGHT / 2 - 3, 0));
    dt::TextComponent* info_text = info_node->AddComponent(new dt::TextComponent("Left player: W/S -- Right player: Up/Down", "text"));
    info_text->SetFont("DejaVuSans");
    info_text->SetFontSize(20);

    ResetBall();
}

void Main::UpdateStateFrame(double simulation_frame_time) {
    mBallSpeed *= 1.0 + (simulation_frame_time * 0.05);

	if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
        dt::StateManager::Get()->Pop();
    }

    // move paddle 1
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W)) {
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0.1f,0));
    }
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S)) {
		mPaddle1Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,-0.1f,0));
    }

    // move paddle 2
    float move2 = 0;
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_UP)) {
        move2 += 1;
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,0.1f,0));
    }

    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_DOWN)) {
        move2 -= 1;
		mPaddle2Node->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(0,-0.1f,0));
    }

    // move ball
	mBallNode->FindComponent<dt::PhysicsBodyComponent>("body")->ApplyCentralImpulse(btVector3(mBallSpeed.x,mBallSpeed.y,mBallSpeed.z));
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
		mBallSpeed *= -1;
	}
	else if (collider->GetNode()->GetName() == "paddle2") {
		mBallSpeed *= -1;
	}
}