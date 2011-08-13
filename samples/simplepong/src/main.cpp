
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/TextComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include <Event/BeginFrameEvent.hpp>
#include <Input/InputManager.hpp>
#include <Core/ResourceManager.hpp>

#include <OgreFontManager.h>
#include <OgreVector3.h>

#include <OgreProcedural.h>

float FIELD_WIDTH = 24;
float FIELD_HEIGHT = 16;
float PADDLE_SIZE = 3.0;

class Main: public dt::State {
public:
    void ResetBall() {
        if(mScore1 < 10 && mScore2 < 10) {
            mBallSpeed = Ogre::Vector3(4, -3, 0);
        } else {
            mBallSpeed = Ogre::Vector3::ZERO;
            QString p(mScore1 == 10 ? "left" : "right");
            GetScene("testscene")->FindChildNode("info")->FindComponent<dt::TextComponent>("text")->SetText("The " + p + " player wins the game.");
        }
        mBallNode->SetPosition(Ogre::Vector3(0,0,0));
        mScore1Text->SetText(dt::Utils::ToString(mScore1));
        mScore2Text->SetText(dt::Utils::ToString(mScore2));

    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            double frame_time = std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

            mBallSpeed *= 1.0 + (frame_time * 0.05);

            // move paddle 1
            float move1 = 0;
            if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W)) {
                move1 += 1;
            }
            if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S)) {
                move1 -= 1;
            }
            float new_y1 = mPaddle1Node->GetPosition().y + move1 * frame_time * 8;
            if(new_y1 > FIELD_HEIGHT / 2 - PADDLE_SIZE / 2)
                new_y1 = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
            else if(new_y1 < - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2)
                new_y1 = - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2;

            mPaddle1Node->SetPosition(Ogre::Vector3(
                                          mPaddle1Node->GetPosition().x,
                                          new_y1,
                                          mPaddle1Node->GetPosition().z));

            // move paddle 2
            float move2 = 0;
            if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_UP)) {
                move2 += 1;
            }
            if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_DOWN)) {
                move2 -= 1;
            }
            float new_y2 = mPaddle2Node->GetPosition().y + move2 * frame_time * 8;
            if(new_y2 > FIELD_HEIGHT / 2 - PADDLE_SIZE / 2)
                new_y2 = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
            else if(new_y2 < - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2)
                new_y2 = - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2;

            mPaddle2Node->SetPosition(Ogre::Vector3(
                                          mPaddle2Node->GetPosition().x,
                                          new_y2,
                                          mPaddle2Node->GetPosition().z));

            // move ball
            Ogre::Vector3 newpos(mBallNode->GetPosition() + mBallSpeed * frame_time);
            if(newpos.y >= FIELD_HEIGHT / 2 - 0.5 || newpos.y <= -FIELD_HEIGHT / 2 + 0.5) {
                mBallSpeed.y *= -1;
            }

            if(newpos.x >= FIELD_WIDTH / 2 - 0.5) {
                float paddle_y = mPaddle2Node->GetPosition().y;
                if(newpos.y < paddle_y - PADDLE_SIZE / 2 - 0.5 || newpos.y > paddle_y + PADDLE_SIZE / 2 + 0.5) {
                    dt::Logger::Get().Info("Player lost!");
                    ++mScore1;
                    ResetBall();
                } else {
                    mBallSpeed.x *= -1;
                }
            } else if(newpos.x <= -FIELD_WIDTH / 2 + 0.5) {
                float paddle_y = mPaddle1Node->GetPosition().y;
                if(newpos.y < paddle_y - PADDLE_SIZE / 2 - 0.5 || newpos.y > paddle_y + PADDLE_SIZE / 2 + 0.5) {
                    dt::Logger::Get().Info("Computer lost!");
                    ++mScore2;
                    ResetBall();
                } else {
                    mBallSpeed.x *= -1;
                }
            }

            mBallNode->SetPosition(mBallNode->GetPosition() + mBallSpeed * frame_time);

            Ogre::Quaternion q;
            q.FromAngleAxis(Ogre::Radian(Ogre::Math::Cos(Ogre::Radian(dt::Root::GetInstance().GetTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_X);
            Ogre::Quaternion w;
            w.FromAngleAxis(Ogre::Radian(Ogre::Math::Sin(Ogre::Radian(dt::Root::GetInstance().GetTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_Y);
            mGameNode->SetRotation(q * w);
        }
    }

    void OnInitialize() {
        mScore1 = 0;
        mScore2 = 0;

        dt::Scene* scene = AddScene(new dt::Scene("testscene"));
        OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

        dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->SetPosition(Ogre::Vector3(0, 0, 30));
        camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
        lightnode->SetPosition(Ogre::Vector3(-20, 20, 10));
        lightnode->AddComponent(new dt::LightComponent("light"));

        // generate meshes
        OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(FIELD_HEIGHT).setSizeZ(1.f).realizeMesh("Field");
        OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(1.f).setSizeZ(1.f).realizeMesh("Ball");
        OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(3.f).setSizeZ(1.f).realizeMesh("Paddle");

        mGameNode = scene->AddChildNode(new dt::Node("game"));

        mFieldNode = mGameNode->AddChildNode(new dt::Node("field"));
        mFieldNode->SetPosition(Ogre::Vector3(0, 0, -1));
        mFieldNode->AddComponent(new dt::MeshComponent("Field", "SimplePongField", "mesh"));

        mBallNode = mGameNode->AddChildNode(new dt::Node("ball"));
        mBallNode->SetPosition(Ogre::Vector3(0, 0, 0));
        mBallNode->AddComponent(new dt::MeshComponent("Ball", "SimplePongBall", "mesh"));

        mPaddle1Node = mGameNode->AddChildNode(new dt::Node("paddle1"));
        mPaddle1Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2 - 0.5, 0, 0));
        mPaddle1Node->AddComponent(new dt::MeshComponent("Paddle", "SimplePongPaddle", "mesh"));

        mPaddle2Node = mGameNode->AddChildNode(new dt::Node("paddle2"));
        mPaddle2Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2 + 0.5, 0, 0));
        mPaddle2Node->AddComponent(new dt::MeshComponent("Paddle", "SimplePongPaddle", "mesh"));

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

private:
    dt::Node* mGameNode;
    dt::Node* mFieldNode;
    dt::Node* mBallNode;
    dt::Node* mPaddle1Node;
    dt::Node* mPaddle2Node;
    dt::TextComponent* mScore1Text;
    dt::TextComponent* mScore2Text;
    int mScore1;
    int mScore2;

    Ogre::Vector3 mBallSpeed;

};

int main(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}
