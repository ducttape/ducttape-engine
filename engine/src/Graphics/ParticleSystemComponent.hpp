
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_PARTICLESYSTEMCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PARTICLESYSTEMCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <OgreParticleEmitter.h>
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>

#include <QString>

#include <map>

namespace dt {

/**
  * Adds a particle system to the scene.
  * @todo More emitter types.
  * @todo Have a look at Emitting emitters: http://www.ogre3d.org/docs/manual/manual_38.html#SEC232
  * @todo Add effector shortcuts.
  */
class DUCTTAPE_API ParticleSystemComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @see Component
      * @param name The name of the component.
      */
    ParticleSystemComponent(const QString& name = "");

    /**
      * Sets the maximum number of particles allowed in this ParticleSystem.
      * @param limit The maximum number of particles allowed.
      */
    void SetParticleCountLimit(uint32_t limit);

    /**
      * Returns the maximum number of particles allowed in this ParticleSystem.
      * @returns The maximum number of particles allowed.
      */
    uint32_t GetParticleCountLimit() const;

    /**
      * Sets the name of the material to use for the particles.
      * @param material_name The name of the material to apply to the particles.
      */
    void SetMaterialName(const QString& material_name);

    /**
      * Gets the name of the material used for the particles.
      * @returns The name of the material applied to the particles.
      */
    const QString& GetMaterialName() const;

    /**
      * Adds an emitter to the particle system.
      * @param name The name for the new emitter.
      * @param type The type of the Ogre emitter.
      * @see http://www.ogre3d.org/docs/manual/manual_36.html#SEC208
      * @returns The newly created particle emitter.
      */
    Ogre::ParticleEmitter* AddEmitter(const QString& name, const QString& type = "Point");

    /**
      * Returns the Emitter for the specified name.
      * @param name The name of the emitter to search.
      * @returns A pointer to the emitter, or nullptr if no emitter with the given name exists.
      */
    Ogre::ParticleEmitter* GetEmitter(const QString& name);

    /**
      * Adds an affector to the particle system.
      * @param name The name for the new affector.
      * @param type The type of the Ogre affector.
      * @see http://www.ogre3d.org/docs/manual/manual_40.html#SEC234
      * @returns The newly created particle affector.
      */
    Ogre::ParticleAffector* AddAffector(const QString& name, const QString& type);

    /**
      * Returns the Affector for the specified name.
      * @param name The name of the affector to search.
      * @returns A pointer to the affector, or nullptr if no affector with the given name exists.
      */
    Ogre::ParticleAffector* GetAffector(const QString& name);

    /////////////////////// Special affectors /////////////////////

    /**
      * Adds an Scaler affector to the particle system.
      * @see http://www.ogre3d.org/docs/manual/manual_40.html#SEC238
      * @see Ogre::ParticleAffector* AddAffector(const QString& name, const QString& type);
      * @param name The name for the new affector.
      * @param rate The scaling rate per second.
      * @returns The newly created particle affector.
      */
    Ogre::ParticleAffector* AddScalerAffector(const QString& name, float rate);

    /**
      * Adds an linear force affector to the particle system.
      * @see http://www.ogre3d.org/docs/manual/manual_40.html#SEC235
      * @see Ogre::ParticleAffector* AddAffector(const QString& name, const QString& type);
      * @param name The name for the new affector.
      * @param force The force vector to apply to all particles.
      * @returns The newly created particle affector.
      */
    Ogre::ParticleAffector* AddLinearForceAffector(const QString& name, Ogre::Vector3 force);

    /**
      * Returns the Ogre particle system.
      * @returns The Ogre particle system.
      */
    Ogre::ParticleSystem* GetOgreParticleSystem();

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

private:
    Ogre::SceneNode* mSceneNode;                //!< The Ogre::SceneNode this particle system is attached to.
    Ogre::ParticleSystem* mParticleSystem;      //!< The Ogre::ParticleSystem instance.
    uint32_t mParticleCountLimit;               //!< The maximum number of particles the system is allowed to have active at once.
    QString mMaterialName;                  //!< The name of the material for the particles.
    std::map<QString, Ogre::ParticleEmitter*> mParticleEmitters;    //!< Used to assign names to particle emitters that are held by the particle system.
    std::map<QString, Ogre::ParticleAffector*> mParticleAffectors;  //!< Used to assign names to particle affectors that are held by the particle system.
};

}

#endif
