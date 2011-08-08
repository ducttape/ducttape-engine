
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIROOTWINDOW
#define DUCTTAPE_ENGINE_GUI_GUIROOTWINDOW

#include <Config.hpp>

#include <Gui/GuiWindow.hpp>

#include <MyGUI.h>

namespace dt {

/**
  * Like GuiWindow, but used in GuiManager as the root widget.
  */
class DUCTTAPE_API GuiRootWindow : public GuiWindow {
    Q_OBJECT
public:
    /**
      * Constructor.
      */
    GuiRootWindow(const QString& name);
    void OnCreate();

};

}

#endif
