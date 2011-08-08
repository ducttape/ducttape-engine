
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIEDITBOX
#define DUCTTAPE_ENGINE_GUI_GUIEDITBOX

#include <Config.hpp>

#include <Gui/GuiTextBox.hpp>

#include <MyGUI.h>

namespace dt {

/**
  * GUI Edit Box widget.
  */
class DUCTTAPE_API GuiEditBox : public GuiTextBox {
    Q_OBJECT

public:
    /**
      * Constructor.
      */
    GuiEditBox(const QString& name);
    MyGUI::Widget* GetMyGUIWidget();
    void OnCreate();

private:
    MyGUI::EditBox* mEditBox;

};

}

#endif
