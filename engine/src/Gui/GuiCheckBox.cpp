
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiCheckBox.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiCheckBox::GuiCheckBox(const QString name)
    : GuiButton(name){}

void GuiCheckBox::onInitialize() {
     mButton = getParent()->getMyGUIWidget()->createWidget<MyGUI::Button>("CheckBox", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
}

}
