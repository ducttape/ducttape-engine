
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiScrollBar.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiScrollBar::GuiScrollBar(const QString name)
    : GuiWidget(name),
      mScrollBar(nullptr) {}

MyGUI::Widget* GuiScrollBar::getMyGUIWidget() {
    return mScrollBar;
}

void GuiScrollBar::onInitialize() {
    mScrollBar = getParent()->getMyGUIWidget()->createWidget<MyGUI::ScrollBar>("ScrollBarH", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
}


void GuiScrollBar::setScrollRange(size_t range) {
    dynamic_cast<MyGUI::ScrollBar*>(getMyGUIWidget())->setScrollRange(range);
	emit scrollRangeChanged(range);
}

size_t GuiScrollBar::getScrollRange() {
    return dynamic_cast<MyGUI::ScrollBar*>(getMyGUIWidget())->getScrollRange();
}


void GuiScrollBar::setScrollPosition(size_t position) {
    dynamic_cast<MyGUI::ScrollBar*>(getMyGUIWidget())->setScrollPosition(position);
	emit scrollPositionChanged(position);
}

size_t GuiScrollBar::getScrollPosition() {
    return dynamic_cast<MyGUI::ScrollBar*>(getMyGUIWidget())->getScrollPosition();
}


}

