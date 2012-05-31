
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiImageBox.hpp>

#include <Utils/Utils.hpp>

namespace dt {

GuiImageBox::GuiImageBox(const QString name)
    : GuiWidget(name),
      mImageBox(nullptr) {}

MyGUI::Widget* GuiImageBox::getMyGUIWidget() {
    return mImageBox;
}

void GuiImageBox::onInitialize() {
    mImageBox = getParent()->getMyGUIWidget()->createWidget<MyGUI::ImageBox>("ImageBox", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
}

void GuiImageBox::setImageTexture (const QString &texture) {
    if(getImageTexture() != texture) {
		dynamic_cast<MyGUI::ImageBox*>(getMyGUIWidget())->setImageTexture(dt::Utils::toStdString(texture));
        emit imageTextureChanged(texture);
    }
}

size_t GuiImageBox::getImageTexture() {
	return dynamic_cast<MyGUI::ImageBox*>(getMyGUIWidget())->getImageIndex();
}


}

