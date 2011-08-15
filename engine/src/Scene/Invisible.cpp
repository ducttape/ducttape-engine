
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Invisible.hpp>

#include <Scene/Node.hpp>
#include <Utils/Logger.hpp>
#include <Graphics/MeshComponent.hpp>

namespace dt {

Invisible::Invisible(const QString& name, const QString& mesh_component_name, const QString& mesh_handle)
    : Component(name),
      mMeshComponentName(mesh_component_name),
      mMeshHandle(mesh_handle) {}

void Invisible::OnCreate() {
    //Add the debug shape mesh
    if(mNode->HasComponent(mMeshComponentName)) {
        Logger::Get().Error("Component named " + mMeshComponentName + 
            " already exists! Cannot create the LightComponent's debug shape using this name!");
        exit(1);
    }

    MeshComponent* mesh_component = new MeshComponent(mMeshHandle, "", mMeshComponentName);
    GetNode()->AddComponent(mesh_component);
	mesh_component->SetCastShadows(false);
	Ogre::Vector3 scale = GetNode()->GetScale();
	scale *= 0.5;
	GetNode()->SetScale(scale);
    mesh_component->Enable();
}

void Invisible::hide() {
	MeshComponent* mesh_component = GetNode()->FindComponent<MeshComponent>(mMeshComponentName);
	if(mesh_component->IsEnabled()) {
		mesh_component->Disable();
	}
}

void Invisible::show() {
	MeshComponent* mesh_component = GetNode()->FindComponent<MeshComponent>(mMeshComponentName);
	if(!mesh_component->IsEnabled()) {
		mesh_component->Enable();
	}
}

}