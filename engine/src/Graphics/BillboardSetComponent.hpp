
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_BILLBOARDCOMPONENT_H
#define DUCTTAPE_ENGINE_BILLBOARDCOMPONENT_H

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <OgreBillboardSet.h>
#include <OgreMaterial.h>
#include <OgreTextureUnitState.h>
#include <OgreSceneNode.h>

#include <Qt/qstring.h>

#include <cstdint>

namespace dt {

/**
  * Component to add a billboard (sprite) to a node.
  */
class DUCTTAPE_API BillboardSetComponent : public Component {
//    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @param pool_size The number of billboards in the set.
      * @param file The image file to load.
      * @see Component
      */
    BillboardSetComponent(const QString& name = "", uint32_t pool_size=20, 
                          const QString& file = "");
    
    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
      * Get the ogre BillboardSet.
      * @return A pointer to the ogre BillboardSet.
      */
    Ogre::BillboardSet* GetOgreBillboardSet() const;
    
    /**
      * Set the texture from a image file.
      * @param file The image file to load.
      */
    void SetTextureFromFile(const QString& file);

    /**
      * Set Billboards to always face the camera.
      */
    void SetFaceCamera();
    
    /**
      * Set BillboardSet to face the camera, rotating
      * around a common vector.
      * @param common_vector The Billboards will rotate around this vector
      */  
    void SetOrientedCommon(const Ogre::Vector3& common_vector);
    
    /**
      * Set Billboards to face the camera, rotating
      * around their own y axis.
      */  
    void SetOrientedSelf();
    
    /**
      * Set Billboards perpendicular to common_vector used as Z axis, 
      * and X, Y axis are determined by up_vector
      * @param common_vector The Billboards will be perpendicular to this vector.
      * @param up_vector The X and Y axis of the billboards are determined by this vector.
      */  
    void SetPerpendicularCommon(const Ogre::Vector3& common_vector,
                               const Ogre::Vector3& up_vector);
    
    /**
      * Set Billboards perpendicular to their own Z axis, 
      * and X, Y axis are determined by up_vector.
      * @param up_vector The X and Y axis of the billboards are determined by this vector.
      */  
    void SetPerpendicularSelf(const Ogre::Vector3& up_vector);  
    
    /**
      * Sets whether or not each Pass renders with depth-buffer checking on or not.
      * @param enabled whether Pass renders with depth-buffer checking.
      */  
    void setDepthCheckEnabled(bool enabled);      
    
protected:
    Ogre::BillboardSet* mBillboardSet; //!< The BillboardSet of this component.
    uint32_t mPoolSize; //!< The number of the billboard in the pool.
    QString mImageFile; //!< The file of the texture to load and to apply to the billboard.
    Ogre::MaterialPtr mMaterialPtr; //!< The pointer to the material used to render billboard.
    Ogre::SceneNode* mSceneNode; //!< The pointer to the ogre SceneNode the BillboardSet is attached to.
    Ogre::TextureUnitState* mTextureUnitState; //!< The pointer to the TextureUnitState of the billboard.
};

}

#endif
