
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/GuiWidgetComponent.hpp>

#include <Graphics/GuiManager.hpp>
#include <Scene/Node.hpp>

namespace dt {

GuiWidgetComponent::GuiWidgetComponent(const QString& type, const QString& skin, const QString& name)
    : Component(name),
      mType(type),
      mSkin(skin),
      mUsesPixelCoordinates(false) {}

MyGUI::WidgetPtr GuiWidgetComponent::GetWidget() {
    return mWidget;
}

void GuiWidgetComponent::OnCreate() {
    mWidget = GuiManager::Get()->GetGuiSystem()->createWidgetT(mType.toStdString(), mSkin.toStdString(),
                                                               10, 10, 100, 100, MyGUI::Align::Default, "Main", mName.toStdString());
}

void GuiWidgetComponent::OnDestroy() {
    GuiManager::Get()->GetGuiSystem()->destroyWidget(mWidget);
}

void GuiWidgetComponent::OnEnable() {
    mWidget->setVisible(true);
}

void GuiWidgetComponent::OnDisable() {
    mWidget->setVisible(false);
}

void GuiWidgetComponent::OnUpdate(double time_diff) {
    if(mUsesPixelCoordinates) {
        mWidget->setCoord(MyGUI::IntCoord(
                    GetNode()->GetPosition().x,
                    GetNode()->GetPosition().y,
                    GetNode()->GetScale().x,
                    GetNode()->GetScale().y));
    } else {
        mWidget->setRealCoord(MyGUI::FloatCoord(
                    GetNode()->GetPosition().x,
                    GetNode()->GetPosition().y,
                    GetNode()->GetScale().x,
                    GetNode()->GetScale().y));
    }
}

void GuiWidgetComponent::SetUsesPixelCoordinates(bool uses_pixel_coordinates) {
    mUsesPixelCoordinates = uses_pixel_coordinates;
}

bool GuiWidgetComponent::GetUsesPixelCoordinates() {
    return mUsesPixelCoordinates;
}

}
