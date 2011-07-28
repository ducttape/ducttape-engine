
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_GUIWIDGETCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_GUIWIDGETCOMPONENT

#include <string>

#define MYGUI_DONT_USE_OBSOLETE
#include <MyGUI.h>

#include "graphics/GuiManager.hpp"
#include "component/Component.hpp"

namespace dt {

/**
  * Adds a GUI widget.
  */
class GuiWidgetComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    GuiWidgetComponent(const std::string& type, const std::string& name = "");

    template <typename T>
    T* GetWidget() {
        return mWidget->castType<T>();
    }

    MyGUI::WidgetPtr GetWidget();

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

    void SetUsesPixelCoordinates(bool uses_pixel_coordinates);
    bool GetUsesPixelCoordinates();

protected:
    std::string mType;
    MyGUI::WidgetPtr mWidget;
    bool mUsesPixelCoordinates;

};

}

#endif
