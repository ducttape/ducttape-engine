#include "graphics/DisplayManager.hpp"

namespace dt {

DisplayManager::DisplayManager() {}
DisplayManager::~DisplayManager() {}
void DisplayManager::Initialize() {}
void DisplayManager::Deinitialize() {}
bool DisplayManager::RegisterCamera(CameraComponent* camera_component) {}
bool DisplayManager::ActivateCamera(const std::string& name) {}
void DisplayManager::_CreateWindow() {}
void DisplayManager::_CreateViewport() {}

}
