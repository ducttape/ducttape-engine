
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_GUIWIDGETCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_GUIWIDGETCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#define MYGUI_DONT_USE_OBSOLETE
#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#include <QString>

namespace dt {

/**
  * Adds a GUI widget to the screen.
  */
class DUCTTAPE_API GuiWidgetComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @param type The type of the MyGUI widget.
      * @param name The name of the component.
      * @see http://mygui.info
      * @see Component
      */
    GuiWidgetComponent(const QString& type, const QString& name = "");

    /**
      * Returns the MyGUI widget.
      * @returns The MyGUI widget.
      */
    template <typename T>
    T* GetWidget() {
        return mWidget->castType<T>();
    }

    /**
      * Returns the MyGUI widget.
      * @returns The MyGUI widget.
      */
    MyGUI::WidgetPtr GetWidget();

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
      * Sets whether the node's coordinates should be interpreted as pixel coordinates.
      * @param uses_pixel_coordinates Whether the node's coordinates should be interpreted as pixel coordinates.
      */
    void SetUsesPixelCoordinates(bool uses_pixel_coordinates);

    /**
      * Gets whether the node's coordinates should be interpreted as pixel coordinates.
      * @returns Whether the node's coordinates should be interpreted as pixel coordinates.
      */
    bool GetUsesPixelCoordinates();

protected:
    QString mType;          //!< The type of the MyGUI widget (e.g. "Button").
    MyGUI::WidgetPtr mWidget;   //!< A pointer to MyGUI's widget.
    bool mUsesPixelCoordinates; //!< Whether the node's coordinates should be interpreted as pixel coordinates.
};

}

#endif
