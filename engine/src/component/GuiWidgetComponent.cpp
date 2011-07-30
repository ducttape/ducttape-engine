#include "GuiWidgetComponent.hpp"

#include "scene/Node.hpp"

namespace dt {

GuiWidgetComponent::GuiWidgetComponent(const std::string& type, const std::string& name)
    : Component(name),
      mType(type),
      mUsesPixelCoordinates(false) {}

MyGUI::WidgetPtr GuiWidgetComponent::GetWidget() {
    return mWidget;
}

void GuiWidgetComponent::OnCreate() {
    mWidget = GuiManager::Get()->GetGuiSystem()->createWidgetT(mType, "Button", 10, 10, 100, 100, MyGUI::Align::Default, "Main", mName);
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
