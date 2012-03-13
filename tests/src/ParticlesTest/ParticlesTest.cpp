
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ParticlesTest/ParticlesTest.hpp"

//#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

#include <OgreParticleAffector.h>

namespace ParticlesTest {

bool ParticlesTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString ParticlesTest::getTestName() {
    return "Particles";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 2.0) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::get()->addResourceLocation("particle/","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // make a scene
    dt::Node* camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 2, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 2, 0));;

    dt::Node* p = scene->addChildNode(new dt::Node("p"));

    p->setScale(0.2);
    dt::MeshComponent* mesh = p->addComponent(new dt::MeshComponent("Sinbad.mesh"));
    mesh->setAnimation("RunBase");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();

    dt::FollowPathComponent* path =
        p->addComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP));
    path->addPoint(Ogre::Vector3(-10, 0, 0));
    path->addPoint(Ogre::Vector3(10, 0, 0));
    path->setDuration(2.f);
    path->setFollowRotation(true);

    // create the particle system
    dt::ParticleSystemComponent* p_sys = p->addComponent(new dt::ParticleSystemComponent("p_sys"));
    p_sys->setMaterialName("Test/Particle");
    p_sys->setParticleCountLimit(1000);
    p_sys->getOgreParticleSystem()->setDefaultDimensions(0.03, 0.03);

    Ogre::ParticleEmitter* e = p_sys->addEmitter("emit1", "Point");
    e->setAngle(Ogre::Degree(10));
    e->setColour(Ogre::ColourValue(1.f, 0.6f, 0.f), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
    e->setEmissionRate(100);
    e->setParticleVelocity(3.f, 4.f);
    e->setTimeToLive(1.f, 2.f);

    p_sys->addScalerAffector("scaler", 1.05);
    p_sys->addLinearForceAffector("force", Ogre::Vector3(0, 5, 0));

    Ogre::ParticleAffector* a = p_sys->addAffector("colour_interpolator", "ColourInterpolator");
    a->setParameter("time0", "0");
    a->setParameter("colour0", "1 1 0 1");
    a->setParameter("time1", "0.5");
    a->setParameter("colour1", "1 0.3 0 1");
    a->setParameter("time2", "1");
    a->setParameter("colour2", "1 0 0 0");
}

}
