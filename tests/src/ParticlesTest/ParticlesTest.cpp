
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ParticlesTest/ParticlesTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

#include <OgreParticleAffector.h>

namespace ParticlesTest {

bool ParticlesTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString ParticlesTest::GetTestName() {
    return "Particles";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 2.0) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::Get()->AddResourceLocation("particle/","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // make a scene
    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 2, 10));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 2, 0));;

    dt::Node* p = scene->AddChildNode(new dt::Node("p"));

    p->SetScale(0.2);
    dt::MeshComponent* mesh = p->AddComponent(new dt::MeshComponent("Sinbad.mesh"));
    mesh->SetAnimation("RunBase");
    mesh->SetLoopAnimation(true);
    mesh->PlayAnimation();

    dt::FollowPathComponent* path =
        p->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP));
    path->AddPoint(Ogre::Vector3(-10, 0, 0));
    path->AddPoint(Ogre::Vector3(10, 0, 0));
    path->SetDuration(2.f);
    path->SetFollowRotation(true);

    // create the particle system
    dt::ParticleSystemComponent* p_sys = p->AddComponent(new dt::ParticleSystemComponent("p_sys"));
    p_sys->SetMaterialName("Test/Particle");
    p_sys->SetParticleCountLimit(1000);
    p_sys->GetOgreParticleSystem()->setDefaultDimensions(0.03, 0.03);

    Ogre::ParticleEmitter* e = p_sys->AddEmitter("emit1", "Point");
    e->setAngle(Ogre::Degree(10));
    e->setColour(Ogre::ColourValue(1.f, 0.6f, 0.f), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
    e->setEmissionRate(100);
    e->setParticleVelocity(3.f, 4.f);
    e->setTimeToLive(1.f, 2.f);

    p_sys->AddScalerAffector("scaler", 1.05);
    p_sys->AddLinearForceAffector("force", Ogre::Vector3(0, 5, 0));

    Ogre::ParticleAffector* a = p_sys->AddAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", "0");
    a->setParameter("colour0", "1 1 0 1");
    a->setParameter("time1", "0.5");
    a->setParameter("colour1", "1 0.3 0 1");
    a->setParameter("time2", "1");
    a->setParameter("colour2", "1 0 0 0");
}

}
