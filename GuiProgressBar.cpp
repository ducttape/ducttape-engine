
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiProgressBar.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiProgressBar::GuiProgressBar(const QString name)
    : GuiWidget(name),
      mProgressBar(nullptr) {}

MyGUI::Widget* GuiProgressBar::getMyGUIWidget() {
    return mProgressBar;
}

void GuiProgressBar::onInitialize() {
	mProgressBar = getParent()->getMyGUIWidget()->createWidget<MyGUI::ProgressBar>("ProgressBar", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
}


void GuiProgressBar::setProgressRange(size_t range) {
	dynamic_cast<MyGUI::ProgressBar*>(getMyGUIWidget())->setProgressRange(range);
	emit progressRangeChanged(range);
}

size_t GuiProgressBar::getProgressRange() {
	return dynamic_cast<MyGUI::ProgressBar*>(getMyGUIWidget())->getProgressRange();
}


void GuiProgressBar::setProgressPosition(size_t position) {
	dynamic_cast<MyGUI::ProgressBar*>(getMyGUIWidget())->setProgressPosition(position);
	emit progressRangeChanged(position);
}

size_t GuiProgressBar::getProgressPosition() {
	return dynamic_cast<MyGUI::ProgressBar*>(getMyGUIWidget())->getProgressPosition();
}


void GuiProgressBar::setProgressAutoTrack(bool _value) {
	dynamic_cast<MyGUI::ProgressBar*>(getMyGUIWidget())->setProgressAutoTrack(_value);
	emit progressRangeChanged(_value);
}

bool GuiProgressBar::getProgressAutoTrack() {
	return dynamic_cast<MyGUI::ProgressBar*>(getMyGUIWidget())->getProgressAutoTrack();
}

}

