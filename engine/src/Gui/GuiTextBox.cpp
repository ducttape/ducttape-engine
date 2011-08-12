
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiTextBox.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiTextBox::GuiTextBox(QString name)
    : GuiWidget(name) {}

GuiTextBox::~GuiTextBox() {}

void GuiTextBox::SetCaption(QString caption) {
    dynamic_cast<MyGUI::TextBox*>(GetMyGUIWidget())->setCaption(dt::Utils::ToStdString(caption));
}

QString GuiTextBox::GetCaption() {
    return QString(dynamic_cast<MyGUI::TextBox*>(GetMyGUIWidget())->getCaption().asUTF8_c_str());
}

}
