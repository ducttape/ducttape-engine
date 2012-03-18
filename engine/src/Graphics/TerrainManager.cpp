
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/TerrainManager.hpp>

#include <Core/Root.hpp>
#include <Utils/Utils.hpp>

#include <SFML/Window/VideoMode.hpp>

#include <cstdint>

namespace dt {

TerrainManager::TextureLayer::TextureLayer(const std::vector<QString>& texture_names, float world_size, float min_height, float fade_distance)
    : mLayerInstance(new Ogre::Terrain::LayerInstance()),
      mMinHeight(min_height),
      mFadeDistance(fade_distance) {
    mLayerInstance->worldSize = world_size;
    for(auto it = texture_names.begin(); it != texture_names.end(); ++it) {
        mLayerInstance->textureNames.push_back(dt::Utils::toStdString(*it));
    }
}

TerrainManager::TextureLayer::~TextureLayer() {
    delete mLayerInstance;
}

Ogre::Terrain::LayerInstance* TerrainManager::TextureLayer::getLayerInstance() const {
    return mLayerInstance;
}

float TerrainManager::TextureLayer::getMinHeight() const {
    return mMinHeight;
}

float TerrainManager::TextureLayer::getFadeDistance() const {
    return mFadeDistance;
}

/////////////////////// END TEXTURE_LAYER /////////////////////////

TerrainManager::TerrainManager()
    : mTerrainGroup(nullptr),
      mTerrainGlobalOptions(nullptr),
      mScene(nullptr),
      mCountX(1),
      mCountY(1),
      mTerrainSize(513),
      mTerrainWorldSize(1200.0f),
      mScale(60.0f),
      mImported(false) {}

TerrainManager::~TerrainManager() {}

void TerrainManager::initialize() {}

void TerrainManager::deinitialize() {
    _destroyTerrain();
}

TerrainManager* TerrainManager::get() {
    return Root::getInstance().getTerrainManager();
}

void TerrainManager::setScene(dt::Scene* scene) {
    _destroyTerrain();
    mScene = scene;
}

void TerrainManager::setLight(dt::LightComponent* light) {
    mLight = light;
}

void TerrainManager::setSize(uint32_t count_x, uint32_t count_y) {
    mCountX = count_x;
    mCountY = count_y;
}

void TerrainManager::setScale(float scale) {
    mScale = scale;
}

Ogre::TerrainGroup* TerrainManager::getOgreTerrainGroup() const {
    return mTerrainGroup;
}

Ogre::TerrainGlobalOptions* TerrainManager::getOgreTerrainGlobalOptions() const {
    return mTerrainGlobalOptions;
}

void TerrainManager::addTextureLayer(const std::vector<QString>& texture_names, float world_size,
                                     float min_height, float fade_distance) {
    TextureLayer::TextureLayerSP texture_layer_sp(new TextureLayer(texture_names, world_size, min_height, fade_distance));
    mTextureLayer.push_back(texture_layer_sp);
}

bool TerrainManager::import(const std::vector<QString>& files) {
    // If we have to less files return false
    if(files.size() < (mCountX*mCountY))
        return false;
    _createTerrain();
    uint16_t i = 0;
    for(uint32_t x = 0; x < mCountX; x++) {
        for(uint32_t y = 0; y < mCountY; y++) {
            _defineTerrain(x, y, files.at(i++));
        }
    }

    mImported = true;
    refresh();
    return true;
}

bool TerrainManager::load(const QString prefix, const QString suffix) {
    _createTerrain();
    mTerrainGroup->setFilenameConvention(dt::Utils::toStdString(prefix), dt::Utils::toStdString(suffix));
    for(long x = 0; x < mCountX; x++) {
        for(long y = 0; y < mCountY; y++) {
            _defineTerrain(x, y);
        }
    }

    mImported = false;
    refresh();
    return true;
}

void TerrainManager::save(const QString prefix, const QString suffix) {
    if(mTerrainGroup == nullptr) return;
    mTerrainGroup->setFilenameConvention(dt::Utils::toStdString(prefix), dt::Utils::toStdString(suffix));
    mTerrainGroup->saveAllTerrains(true);
}

void TerrainManager::refresh() {
    mTerrainGroup->loadAllTerrains(true);

    if(mImported) {
        Ogre::TerrainGroup::TerrainIterator it = mTerrainGroup->getTerrainIterator();
        while(it.hasMoreElements())
        {
            Ogre::Terrain* terrain = it.getNext()->instance;
            _generateBlendMaps(terrain);
        }
    }

    mTerrainGroup->freeTemporaryResources();
}

void TerrainManager::_createTerrain() {
    // destroy existing terrain and global-options
    _destroyTerrain();

    _initOptions();

    Ogre::SceneManager* scene_mgr = mScene->getSceneManager();

    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(scene_mgr, Ogre::Terrain::ALIGN_X_Z, mTerrainSize, mTerrainWorldSize);
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO); // TODO: let the user move it where he wants it. (Is this necessary?)

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& import_data = mTerrainGroup->getDefaultImportSettings();

    import_data.inputScale = mScale;
    import_data.minBatchSize = 33;
    import_data.maxBatchSize = 65;

    // textures
    for(auto it = mTextureLayer.begin(); it != mTextureLayer.end(); ++it) {
        import_data.layerList.push_back(*(it->get()->getLayerInstance()));
    }
}

void TerrainManager::_initOptions() {
    if(mTerrainGlobalOptions == nullptr)
        mTerrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions();

    Ogre::SceneManager* scene_mgr = mScene->getSceneManager();

    // Configure global
    mTerrainGlobalOptions->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobalOptions->setCompositeMapDistance(12000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobalOptions->setCompositeMapAmbient(scene_mgr->getAmbientLight());
    if(mLight != nullptr) {
        mTerrainGlobalOptions->setLightMapDirection(mLight->getOgreLight()->getDerivedDirection());
        mTerrainGlobalOptions->setCompositeMapDiffuse(mLight->getOgreLight()->getDiffuseColour());
    }
}

void TerrainManager::_destroyTerrain() {
    if(mTerrainGroup != nullptr) {
        OGRE_DELETE mTerrainGroup;
        mTerrainGroup = nullptr;
    }

    if(mTerrainGlobalOptions != nullptr) {
        OGRE_DELETE mTerrainGlobalOptions;
        mTerrainGlobalOptions = nullptr;
    }
}

void TerrainManager::_defineTerrain(uint32_t x, uint32_t y) {
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if(Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename)) {
        mTerrainGroup->defineTerrain(x, y);
    } // TODO: else
}
void TerrainManager::_defineTerrain(uint32_t x, uint32_t y, const QString filename) {
    Ogre::Image img;
    img.load(dt::Utils::toStdString(filename), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); //TODO: resourcegroup???
    mTerrainGroup->defineTerrain(x, y, &img);
}

void TerrainManager::_generateBlendMaps(Ogre::Terrain* terrain) {
    if(mTextureLayer.size() < 2) return;
    for(uint32_t i = 1; i < mTextureLayer.size()-1; i++) {
        TextureLayer::TextureLayerSP& layer = mTextureLayer[i];
        Ogre::TerrainLayerBlendMap* blend_map = terrain->getLayerBlendMap(i);
        float* blend_ptr = blend_map->getBlendPointer();
        for(uint32_t y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
            for(uint32_t x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
                float tx, ty;

                blend_map->convertImageToTerrainSpace(x, y, &tx, &ty);
                float height = terrain->getHeightAtTerrainPosition(tx, ty);

                float val = (height - layer->getMinHeight()) / layer->getFadeDistance();
                val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
                *blend_ptr++ = val;
            }
        }
    }

}

}

