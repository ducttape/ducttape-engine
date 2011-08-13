
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GRAPHICS_TERRAINMANAGER
#define DUCTTAPE_ENGINE_GRAPHICS_TERRAINMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Graphics/LightComponent.hpp>
#include <Scene/Scene.hpp>

#include <OgreSceneManager.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <QString>
#include <boost/ptr_container/ptr_vector.hpp>

namespace dt {

/**
  * A manager class for managing terrain.
  */
class DUCTTAPE_API TerrainManager : public Manager {
    Q_OBJECT
public:

    /**
      * A simple wrapper for texture-layers
      */
    class DUCTTAPE_API TextureLayer {
    public:
        TextureLayer(const std::vector<QString>& texture_names, float world_size, float min_height, float fade_distance);
        ~TextureLayer();
        Ogre::Terrain::LayerInstance* getLayerInstance() const;
        float getMinHeight() const;
        float getFadeDistance() const;
    private:
        Ogre::Terrain::LayerInstance* mLayerInstance;
        float mMinHeight;
        float mFadeDistance;
    };

    /**
      * Default constructor.
      */
    TerrainManager();

    /**
      * Default destructor.
      */
    ~TerrainManager();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static TerrainManager* Get();

    /**
      * Sets the scene.
      * @param scene The scene.
      */
    void SetScene(dt::Scene* scene);

    /**
      * Sets the light which is used to build the lightmap.
      * @param light The light.
      * @todo getLight(), fallback if not used e.g. default light
      */
    void SetLight(dt::LightComponent* light);

    /**
      * Sets the number of terrains in each direction
      * @todo rename: names misleading.
      */
    void SetSize(uint32_t count_x, uint32_t count_y);

    /**
      * Sets the scale-factor which is used to scale the height when you import from an image.
      */
    void SetScale(float scale);

    /**
      * Gets the Ogre::TerrainGroup.
      * @returns The Ogre::TerrainGroup representing this terrain.
      */
    Ogre::TerrainGroup* GetOgreTerrainGroup() const;

    /**
      * Gets the Ogre::TerrainGlobalOptions.
      * @returns The GlobalOptions for the terrain
      */
    Ogre::TerrainGlobalOptions* GetOgreTerrainGlobalOptions() const;

    /**
      * Adds a TextureLayer.
      * @param texture_layer The Layer.
      */
    void AddTextureLayer(const std::vector<QString>& texture_names, float world_size, float min_height, float fade_distance);

    /**
      * Imports a terrain using the images
      * @param files The filenames of the images to be used.
      * @returns true on success, false otherwise.
      */
    bool Import(const std::vector<QString>& files);

    /**
      * Loads an existing terrain using the file-prefix and suffix
      * (<prefix><index><suffix>)
      * @param prefix the filenames prefix
      * @param suffix the filenames suffix
      * @returns true on success, false otherwise.
      */
    bool Load(const QString& prefix, const QString& suffix);

    /**
      * Saves the terrain to disk using the file-prefix and suffix
      * (<prefix><index><suffix>)
      * @param prefix the filenames prefix
      * @param suffix the filenames suffix
      */
    void Save(const QString& prefix, const QString& suffix);

    /**
      * Refreshes the terrain/Applys all changes.
      */
    void Refresh();

private:
    /**
      * Private method. Creates the TerrainGroup-object.
      */
    void _CreateTerrain();

    /**
      * Private method. Initializes the Options with default values. Creates the TerrainGlobalOptions-object if necessary.
      */
    void _InitOptions();

    /**
      * Private method. Destroys the terrain.
      */
    void _DestroyTerrain();

    /**
      * Private method. Load a part of the terrain.
      */
    void _DefineTerrain(uint32_t x, uint32_t y);

    /**
      * Private method. Import a part of the terrain.
      */
    void _DefineTerrain(uint32_t x, uint32_t y, const QString& filename);

    /**
      * Private method. Creates the blendmaps for a terrain.
      */
    void _GenerateBlendMaps(Ogre::Terrain* terrain);

    boost::ptr_vector<TextureLayer> mTextureLayer;          //!< The TextureLayers.
    dt::LightComponent* mLight;                             //!< The light to bake the lightmap
    Ogre::TerrainGroup* mTerrainGroup;                      //!< The terraingroup which holds our terrain.
    Ogre::TerrainGlobalOptions* mTerrainGlobalOptions;      //!< The global options for terrains.
    dt::Scene* mScene;                                      //!< The scene.

    uint32_t mCountX, mCountY;                              //!< The number of terrains in x and y (Ogre-Z) coordinate.
    uint32_t mTerrainSize;                                  //!< The size of one terrain. (mTerrainSize*mTerrainSize vertices) Must be 2^n+1.
    float mTerrainWorldSize;                                //!< The world size of one terrain. This is the scale of the terrain and depends on the size of the other entities.
    float mScale;                                           //!< Factor which is used to scale the imported height to your needs.

    bool mImported;                                         //!< Indicates whether the scene was imported (from image) or loaded from ogres filetype.
};

}

#endif
