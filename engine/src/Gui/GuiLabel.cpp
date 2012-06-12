
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiLabel.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiLabel::GuiLabel(const QString name)
    : GuiTextBox(name),
      mLabel(nullptr) {}

MyGUI::Widget* GuiLabel::getMyGUIWidget() {
    return mLabel;
}

void GuiLabel::onInitialize() {
    mLabel = getParent()->getMyGUIWidget()->createWidget<MyGUI::TextBox>("Label", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
}

}
