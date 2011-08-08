
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiWidget.hpp>

#include <Gui/GuiManager.hpp>
#include <Logic/ScriptManager.hpp>
#include <Utils/Logger.hpp>

namespace dt {

GuiWidget::GuiWidget(const QString& name)
    : mName(name),
      mParent(nullptr),
      mIsVisible(true) {}

GuiWidget::~GuiWidget() {}

void GuiWidget::Create() {
    GuiManager::Get()->Initialize(); // initialize if not already happened
    OnCreate();
}

const QString& GuiWidget::GetName() const {
    return mName;
}

void GuiWidget::Focus() {
    MyGUI::InputManager::getInstance().setKeyFocusWidget(GetMyGUIWidget());
}

void GuiWidget::SetPosition(float x, float y) {
    GetMyGUIWidget()->setRealPosition(x, y);
}

void GuiWidget::SetPosition(int x, int y) {
    GetMyGUIWidget()->setPosition(x, y);
}

void GuiWidget::SetSize(float width, float height) {
    GetMyGUIWidget()->setRealSize(width, height);
}

void GuiWidget::SetSize(int width, int height) {
    GetMyGUIWidget()->setSize(width, height);
}

// Parent management

void GuiWidget::SetParent(GuiWidget* parent) {
    if(mParent != nullptr) {
        // move ourselves to another widget
        boost::ptr_map<QString, GuiWidget>& from = mParent->GetChildrenMap();
        boost::ptr_map<QString, GuiWidget>& to = parent->GetChildrenMap();
        to.transfer(to.end(), from.find(mName), from);
    }
    mParent = parent;
}

GuiWidget* GuiWidget::GetParent() {
    if(mParent == nullptr) {
        // uh oh!
        Logger::Get().Error("Parent of widget " + GetFullName() + " is nullptr.");
    }
    return mParent;
}

QScriptValue GuiWidget::GetScriptParent() {
    return dt::ScriptManager::Get()->GetScriptEngine()->newQObject(mParent);
}

void GuiWidget::SetScriptParent(QScriptValue parent) {
    QObject* q = parent.toQObject();
    mParent = dynamic_cast<GuiWidget*>(q);
}

// Children management

GuiWidget* GuiWidget::FindChild(const QString& name) {
    boost::ptr_map<QString, GuiWidget>::iterator iter = mChildren.find(name);
    if(iter == mChildren.end()) {
        return nullptr;
    }
    return iter->second;
}

QScriptValue GuiWidget::GetChild(const QString& name) {
    GuiWidget* widget = FindChild(name);
    if(widget == nullptr) {
        return QScriptValue(dt::ScriptManager::Get()->GetScriptEngine(), QScriptValue::UndefinedValue);
    }
    return dt::ScriptManager::Get()->GetScriptEngine()->newQObject(widget);
}

void GuiWidget::Show() {
    SetVisible(true);
}

void GuiWidget::Hide() {
    SetVisible(false);
}

void GuiWidget::SetVisible(bool visible) {
    mIsVisible = visible;
    GetMyGUIWidget()->setVisible(visible);
}

bool GuiWidget::IsVisible() const {
    return mIsVisible;
}

boost::ptr_map<QString, GuiWidget>& GuiWidget::GetChildrenMap() {
    return mChildren;
}

bool GuiWidget::_AddChild(GuiWidget* widget) {
    QString name(widget->GetName()); // need a copy for boost::ptr_map::insert()

    if(FindChild(name) != nullptr) {
        // widget already exists
        Logger::Get().Error("Cannot add widget \"" + widget->GetName() + "\" to \"" + GetFullName() + "\": widget with this name already exists.");
        return false;
    }

    mChildren.insert(name, widget);
    FindChild(name)->SetParent(this);
    return true;
}

QString GuiWidget::GetFullName() {
    if(mParent == nullptr)
        return mName;
    else
        return mParent->GetFullName() + "." + mName;
}

}
