
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUITEXTBOX
#define DUCTTAPE_ENGINE_GUI_GUITEXTBOX

#include <Config.hpp>

#include <Gui/GuiWidget.hpp>

#include <MyGUI.h>

namespace dt {

/**
  * Base class widget for widgets containing text (GuiButton, GuiEditBox, GuiWindow).
  * If you are looking for an input box widget, try GuiEditBox.
  */
class DUCTTAPE_API GuiTextBox : public GuiWidget {
    Q_OBJECT
    Q_PROPERTY(QString caption READ GetCaption WRITE SetCaption)

public:
    /**
      * Constructor.
      * @param name The name of this widget.
      */
    GuiTextBox(QString name);

    /**
      * Pure virtual destructor.
      */
    virtual ~GuiTextBox() = 0;

    /**
      * Sets the caption of the widget.
      * @param caption The new caption.
      */
    void SetCaption(QString caption);

    /**
      * Gets the caption of the widget.
      * @returns The caption.
      */
    QString GetCaption();

};

}

#endif
