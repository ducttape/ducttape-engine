
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

void GuiTextBox::setCaption(QString caption) {
    if(getCaption() != caption) {
        dynamic_cast<MyGUI::TextBox*>(getMyGUIWidget())->setCaption(dt::Utils::toStdString(caption));
        emit captionChanged(caption);
    }
}

QString GuiTextBox::getCaption() {
    return QString(dynamic_cast<MyGUI::TextBox*>(getMyGUIWidget())->getCaption().asUTF8_c_str());
}

}
