
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiEditBox.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiEditBox::GuiEditBox(const QString& name)
    : GuiTextBox(name),
      mEditBox(nullptr) {}

MyGUI::Widget* GuiEditBox::GetMyGUIWidget() {
    return mEditBox;
}

void GuiEditBox::OnCreate() {
    mEditBox = GetParent()->GetMyGUIWidget()->createWidget<MyGUI::EditBox>("EditBox", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::ToStdString(GetFullName()));
}

}
