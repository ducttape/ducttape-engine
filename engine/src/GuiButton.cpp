
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiButton.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiButton::GuiButton(const QString name)
    : GuiTextBox(name),
      mButton(nullptr) {}

MyGUI::Widget* GuiButton::getMyGUIWidget() {
    return mButton;
}

void GuiButton::onInitialize() {
    mButton = getParent()->getMyGUIWidget()->createWidget<MyGUI::Button>("Button", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
}

void GuiButton::setStateSelected(bool _value) {
    dynamic_cast<MyGUI::Button*>(getMyGUIWidget())->setStateSelected(_value);
    emit stateSelectedChanged(_value);
}

bool GuiButton::getStateSelected() {
    return dynamic_cast<MyGUI::Button*>(getMyGUIWidget())->getStateSelected();
}

}
