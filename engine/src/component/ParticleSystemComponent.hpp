
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_PARTICLESYSTEMCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PARTICLESYSTEMCOMPONENT

#include <string>

#include <OgreParticle.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleSystem.h>
#include <OgreParticleAffector.h>

#include "component/Component.hpp"

namespace dt {

/**
  * Adds a particle system to the scene.
  * @todo More emitter types.
  * @todo Have a look at Emitting emitters: http://www.ogre3d.org/docs/manual/manual_38.html#SEC232
  * @todo Add effector shortcuts.
  */
class ParticleSystemComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      * @param name The name of the component.
      */
    ParticleSystemComponent(const std::string& name);

    void SetParticleCountLimit(uint32_t limit);
    uint32_t GetParticleCountLimit() const;

    void SetMaterialName(const std::string& material_name);
    const std::string& GetMaterialName() const;

    Ogre::ParticleEmitter* AddEmitter(const std::string& name, const std::string& type = "Point");
    Ogre::ParticleEmitter* GetEmitter(const std::string& name);

    Ogre::ParticleAffector* AddAffector(const std::string& name, const std::string& type);
    Ogre::ParticleAffector* GetAffector(const std::string& name);

    Ogre::ParticleSystem* GetOgreParticleSystem();

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(float time_diff);

private:
    Ogre::SceneNode* mSceneNode;                //!< The Ogre::SceneNode this particle system is attached to.
    Ogre::ParticleSystem* mParticleSystem;      //!< The Ogre::ParticleSystem instance.
    uint32_t mParticleCountLimit;               //!< The maximum number of particles the system is allowed to have active at once.
    std::string mMaterialName;                  //!< The name of the material for the particles.
    std::map<std::string, Ogre::ParticleEmitter*> mParticleEmitters; //!< Used to assign names to particle emitters that are held by the particle system.
    std::map<std::string, Ogre::ParticleAffector*> mParticleAffectors; //!< Used to assign names to particle affectors that are held by the particle system.
};

}

#endif
