
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
#include "component/FollowPathComponent.hpp"
#include "component/ParticleSystemComponent.hpp"
#include "event/EventListener.hpp"

class Game : public dt::Game, public dt::EventListener {
public:
    Game()
        : mScene("gamescene") {
        mRuntime = 0;
    }

    void HandleEvent(dt::Event* e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += ((dt::BeginFrameEvent*)e)->GetFrameTime();
            if(mRuntime > 6.0) {
                RequestShutdown();
            }
        }
    }

    void OnInitialize() {
        dt::Root::get_mutable_instance().GetEventManager()->AddListener(&mScene);

        dt::Root::get_mutable_instance().GetResourceManager()->AddResourceLocation("sinbad.zip","Zip", true);
        dt::Root::get_mutable_instance().GetResourceManager()->AddResourceLocation("particle/","FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

        // make a scene
        mScene.AddChildNode(new dt::Node("camnode"));
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::CameraComponent("cam"));
        mScene.FindChildNode("camnode", false)->SetPosition(Ogre::Vector3(0, 2, 10));
        mScene.FindChildNode("camnode", false)->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 2, 0));

        mScene.AddChildNode(new dt::Node("p"));
        dt::Node* p = mScene.FindChildNode("p", false);

        p->SetScale(0.2);
        p->AddComponent(new dt::MeshComponent("lolmesh", "Sinbad.mesh"));
        dt::MeshComponent* mesh = p->FindComponent<dt::MeshComponent>("lolmesh");
        mesh->SetAnimation("RunBase");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();

        p->AddComponent(new dt::FollowPathComponent("path", dt::FollowPathComponent::LOOP));
        dt::FollowPathComponent* path = p->FindComponent<dt::FollowPathComponent>("path");
        path->AddPoint(Ogre::Vector3(-10, 0, 0));
        path->AddPoint(Ogre::Vector3(10, 0, 0));
        path->SetDuration(2.f);
        path->SetFollowRotation(true);

        // create the particle system
        p->AddComponent(new dt::ParticleSystemComponent("p_sys"));
        dt::ParticleSystemComponent* p_sys = p->FindComponent<dt::ParticleSystemComponent>("p_sys");

        p_sys->SetMaterialName("Test/Particle");
        p_sys->SetParticleCountLimit(1000);
        p_sys->GetOgreParticleSystem()->setDefaultDimensions(0.03, 0.03);

        Ogre::ParticleEmitter* e = p_sys->AddEmitter("emit1", "Point");
        e->setAngle(Ogre::Degree(10));
        e->setColour(Ogre::ColourValue(1.f, 0.6f, 0.f), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
        e->setEmissionRate(100);
        e->setParticleVelocity(3.f, 4.f);
        e->setTimeToLive(1.f, 2.f);

        Ogre::ParticleAffector* a = p_sys->AddAffector("scaler", "Scaler");
        a->setParameter("rate", "1.05");

        a = p_sys->AddAffector("colour_interpolator", "ColourInterpolator");
        a->setParameter("time0", "0");
        a->setParameter("colour0", "1 1 0 1");
        a->setParameter("time1", "0.5");
        a->setParameter("colour1", "1 0.3 0 1");
        a->setParameter("time2", "1");
        a->setParameter("colour2", "1 0 0 0");

        a = p_sys->AddAffector("linear_force", "LinearForce");
        a->setParameter("force_vector", "0 5 0");
    }

private:
    double mRuntime;
    dt::Scene mScene;

};

int main(int argc, char** argv) {
    Game g;
    g.Run(argc, argv);
    return 0;
}
