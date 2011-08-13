
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiWindow.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiWindow::GuiWindow(const QString& name)
    : GuiTextBox(name),
      mWindow(nullptr) {}

MyGUI::Widget* GuiWindow::GetMyGUIWidget() {
    return mWindow;
}

void GuiWindow::OnCreate() {
    mWindow = GetParent()->GetMyGUIWidget()->createWidget<MyGUI::Window>("WindowCS", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::ToStdString(GetFullName()));
}

}
