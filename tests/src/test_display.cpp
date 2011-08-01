
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"
#include "component/MeshComponent.hpp"
#include "component/LightComponent.hpp"
#include "event/EventListener.hpp"

class Main: public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 2.5) {
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 5, 10));
        cam->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
        meshnode->AddComponent(mesh);
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();
        mesh->SetCastShadows(true);

        dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
        dt::LightComponent* light = lightnode->AddComponent(new dt::LightComponent("light"));
        Ogre::Light* ogl = light->GetOgreLight();
        ogl->setType(Ogre::Light::LT_SPOTLIGHT);
        ogl->setSpotlightInnerAngle(Ogre::Degree(30));
        ogl->setSpotlightOuterAngle(Ogre::Degree(50));
        lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

        dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
        dt::LightComponent* light2 = lightnode2->AddComponent(new dt::LightComponent("light2"));
        Ogre::Light* ogl2 = light2->GetOgreLight();
        ogl2->setType(Ogre::Light::LT_SPOTLIGHT);
        ogl2->setSpotlightInnerAngle(Ogre::Degree(30));
        ogl2->setSpotlightOuterAngle(Ogre::Degree(50));
        lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));
    }

private:
    double mRuntime;
};

int main(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}
