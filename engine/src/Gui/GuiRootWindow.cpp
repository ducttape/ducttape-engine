
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiRootWindow.hpp>

#include <Gui/GuiManager.hpp>
#include <Utils/Utils.hpp>

namespace dt {

GuiRootWindow::GuiRootWindow(const QString& name)
    : GuiWindow(name) {}

void GuiRootWindow::OnCreate() {
    mWindow = GuiManager::Get()->GetGuiSystem()->createWidget<MyGUI::Window>("PanelEmpty", 0, 0, 1, 1, MyGUI::Align::Default, "Main", dt::Utils::ToStdString(GetFullName()));
    SetPosition(0, 0);
    SetSize(1.f, 1.f);
}

}
