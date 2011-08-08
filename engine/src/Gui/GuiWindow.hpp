
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIWINDOW
#define DUCTTAPE_ENGINE_GUI_GUIWINDOW

#include <Config.hpp>

#include <Gui/GuiTextBox.hpp>

#include <MyGUI.h>

namespace dt {

/**
  * Basic GuiWidget that manages multiple subwidgets with window-like behaviour.
  */
class DUCTTAPE_API GuiWindow : public GuiTextBox {
    Q_OBJECT

public:
    /**
      * Constructor.
      */
    GuiWindow(const QString& name);
    MyGUI::Widget* GetMyGUIWidget();
    virtual void OnCreate();

protected:
    MyGUI::Window* mWindow;

};

}

#endif
