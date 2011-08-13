
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
        mLayerInstance->textureNames.push_back(dt::Utils::ToStdString(*it));
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

void TerrainManager::Initialize() {}

void TerrainManager::Deinitialize() {
    _DestroyTerrain();
}

TerrainManager* TerrainManager::Get() {
    return Root::GetInstance().GetTerrainManager();
}

void TerrainManager::SetScene(dt::Scene* scene) {
    _DestroyTerrain();
    mScene = scene;
}

void TerrainManager::SetLight(dt::LightComponent* light) {
    mLight = light;
}

void TerrainManager::SetSize(uint32_t count_x, uint32_t count_y) {
    mCountX = count_x;
    mCountY = count_y;
}

void TerrainManager::SetScale(float scale) {
    mScale = scale;
}

Ogre::TerrainGroup* TerrainManager::GetOgreTerrainGroup() const {
    return mTerrainGroup;
}

Ogre::TerrainGlobalOptions* TerrainManager::GetOgreTerrainGlobalOptions() const {
    return mTerrainGlobalOptions;
}

void TerrainManager::AddTextureLayer(const std::vector<QString>& texture_names, float world_size, float min_height, float fade_distance) {
    mTextureLayer.push_back(new TextureLayer(texture_names, world_size, min_height, fade_distance));
}

bool TerrainManager::Import(const std::vector<QString>& files) {
    // If we have to less files return false
    if(files.size() < (mCountX*mCountY))
        return false;
    _CreateTerrain();
    uint16_t i = 0;
    for(uint32_t x = 0; x < mCountX; x++) {
        for(uint32_t y = 0; y < mCountY; y++) {
            _DefineTerrain(x, y, files.at(i++));
        }
    }

    mImported = true;
    Refresh();
    return true;
}

bool TerrainManager::Load(const QString& prefix, const QString& suffix) {
    _CreateTerrain();
    mTerrainGroup->setFilenameConvention(dt::Utils::ToStdString(prefix), dt::Utils::ToStdString(suffix));
    for(long x = 0; x < mCountX; x++) {
        for(long y = 0; y < mCountY; y++) {
            _DefineTerrain(x, y);
        }
    }

    mImported = false;
    Refresh();
    return true;
}

void TerrainManager::Save(const QString& prefix, const QString& suffix) {
    if(mTerrainGroup == nullptr) return;
    mTerrainGroup->setFilenameConvention(dt::Utils::ToStdString(prefix), dt::Utils::ToStdString(suffix));
    mTerrainGroup->saveAllTerrains(true);
}

void TerrainManager::Refresh() {
    mTerrainGroup->loadAllTerrains(true);

    if(mImported) {
        Ogre::TerrainGroup::TerrainIterator it = mTerrainGroup->getTerrainIterator();
        while(it.hasMoreElements())
        {
            Ogre::Terrain* terrain = it.getNext()->instance;
            _GenerateBlendMaps(terrain);
        }
    }

    mTerrainGroup->freeTemporaryResources();
}

void TerrainManager::_CreateTerrain() {
    // destroy existing terrain and global-options
    _DestroyTerrain();

    _InitOptions();

    Ogre::SceneManager* scene_mgr = mScene->GetSceneManager();

    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(scene_mgr, Ogre::Terrain::ALIGN_X_Z, mTerrainSize, mTerrainWorldSize);
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO); // TODO: let the user move it where he wants it. (Is this necessary?)

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& import_data = mTerrainGroup->getDefaultImportSettings();

    import_data.inputScale = mScale;
    import_data.minBatchSize = 33;
    import_data.maxBatchSize = 65;

    // textures
    for(auto it = mTextureLayer.begin(); it != mTextureLayer.end(); ++it) {
        import_data.layerList.push_back(*(it->getLayerInstance()));
    }
}

void TerrainManager::_InitOptions() {
    if(mTerrainGlobalOptions == nullptr)
        mTerrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions();

    Ogre::SceneManager* scene_mgr = mScene->GetSceneManager();

    // Configure global
    mTerrainGlobalOptions->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobalOptions->setCompositeMapDistance(12000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobalOptions->setCompositeMapAmbient(scene_mgr->getAmbientLight());
    if(mLight != nullptr) {
        mTerrainGlobalOptions->setLightMapDirection(mLight->GetOgreLight()->getDerivedDirection());
        mTerrainGlobalOptions->setCompositeMapDiffuse(mLight->GetOgreLight()->getDiffuseColour());
    }
}

void TerrainManager::_DestroyTerrain() {
    if(mTerrainGroup != nullptr) {
        OGRE_DELETE mTerrainGroup;
        mTerrainGroup = nullptr;
    }

    if(mTerrainGlobalOptions != nullptr) {
        OGRE_DELETE mTerrainGlobalOptions;
        mTerrainGlobalOptions = nullptr;
    }
}

void TerrainManager::_DefineTerrain(uint32_t x, uint32_t y) {
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if(Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename)) {
        mTerrainGroup->defineTerrain(x, y);
    } // TODO: else
}
void TerrainManager::_DefineTerrain(uint32_t x, uint32_t y, const QString& filename) {
    Ogre::Image img;
    img.load(dt::Utils::ToStdString(filename), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); //TODO: resourcegroup???
    mTerrainGroup->defineTerrain(x, y, &img);
}

void TerrainManager::_GenerateBlendMaps(Ogre::Terrain* terrain) {
    if(mTextureLayer.size() < 2) return;
    for(uint32_t i = 1; i < mTextureLayer.size()-1; i++) {
        TextureLayer layer = mTextureLayer[i];
        Ogre::TerrainLayerBlendMap* blend_map = terrain->getLayerBlendMap(i);
        float* blend_ptr = blend_map->getBlendPointer();
        for(uint32_t y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
            for(uint32_t x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
                float tx, ty;

                blend_map->convertImageToTerrainSpace(x, y, &tx, &ty);
                float height = terrain->getHeightAtTerrainPosition(tx, ty);

                float val = (height - layer.getMinHeight()) / layer.getFadeDistance();
                val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
                *blend_ptr++ = val;
            }
        }
    }

}

}

