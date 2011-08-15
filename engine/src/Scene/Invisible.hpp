
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_INVISIBLE
#define DUCTTAPE_ENGINE_COMPONENT_INVISIBLE

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <QString>

namespace dt {

/*
 * A abstract class for the invisible components such as LightComponent or SoundComponent.
 */
class DUCTTAPE_API Invisible : public Component {
    Q_OBJECT
public:
	/**
     * Advanced constructor.
     * @param name The name for this component.
     * @param mesh_component_name The name for this component's debug shape mesh.
	 * @param mesh_handle The debug shape mesh's handle.
     * @see Component
     */
    Invisible(const QString& name, const QString& mesh_component_name, const QString& mesh_handle);

    virtual void OnCreate();
	
	/*
	 * Hide the debug shape.
	 */
	void hide();

	/*
	 * Show the debug shape.
	 */
	void show();

protected:
    QString mMeshHandle;            //!< The handle of the mesh.
    QString mMeshComponentName;     //!< The name of the mesh component.
};
}

#endif