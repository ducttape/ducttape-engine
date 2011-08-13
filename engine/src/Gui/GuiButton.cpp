
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiButton.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiButton::GuiButton(const QString& name)
    : GuiTextBox(name),
      mButton(nullptr) {}

MyGUI::Widget* GuiButton::GetMyGUIWidget() {
    return mButton;
}

void GuiButton::OnCreate() {
    mButton = GetParent()->GetMyGUIWidget()->createWidget<MyGUI::Button>("Button", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::ToStdString(GetFullName()));
}

}
