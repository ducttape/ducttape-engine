
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

#include <QStringList>

namespace dt {

GuiWidget::GuiWidget(const QString& name)
    : mName(name),
      mParent(nullptr),
      mIsVisible(true) {

    if(mName.contains('.')) {
        Logger::Get().Warning("GuiWidget name cannot contain '.': \"" + mName + "\". All occurrences will be stripped.");
        mName = mName.replace('.', "");
        Logger::Get().Info("New widget name: \"" + mName + "\".");
    }
}

GuiWidget::~GuiWidget() {}

void GuiWidget::Create() {
    GuiManager::Get()->Initialize(); // initialize if not already happened
    OnCreate();
}

void GuiWidget::Destroy() {
    // destroy all children
    for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
        iter->second->Destroy();

        iter = mChildren.erase(iter);
    }

    // destroy the mygui widget
    GuiManager::Get()->GetGuiSystem()->destroyWidget(GetMyGUIWidget());
}

const QString& GuiWidget::GetName() const {
    return mName;
}

void GuiWidget::Focus() {
    if(MyGUI::InputManager::getInstance().getKeyFocusWidget() != GetMyGUIWidget()) {
        MyGUI::InputManager::getInstance().setKeyFocusWidget(GetMyGUIWidget());
        emit Focused();
    }
}

void GuiWidget::SetPosition(float x, float y) {
    if(GetMyGUIWidget()->getAbsoluteLeft() != (int)x || GetMyGUIWidget()->getAbsoluteTop() != (int)y) {
        GetMyGUIWidget()->setRealPosition(x, y);
        emit PositionChanged(x, y);
    }
}

void GuiWidget::SetPosition(int x, int y) {
    if(GetMyGUIWidget()->getAbsoluteLeft() != x || GetMyGUIWidget()->getAbsoluteTop() != y) {
        GetMyGUIWidget()->setPosition(x, y);
        emit PositionChanged(x, y);
    }
}

void GuiWidget::SetSize(float width, float height) {
    //DO NOT ADD THE TEST BELOW! OR YOU WILL NEVER SEE THE GUI!
    //if(GetMyGUIWidget()->getSize().width != width || GetMyGUIWidget()->getSize().height != height) {
        GetMyGUIWidget()->setRealSize(width, height);
        emit SizeChanged(width, height);
    //}
}

void GuiWidget::SetSize(int width, int height) {
    if(GetMyGUIWidget()->getSize().width != width || GetMyGUIWidget()->getSize().height != height) {
        GetMyGUIWidget()->setSize(width, height);
        emit SizeChanged(width, height);
    }
}

// Parent management

void GuiWidget::SetParent(GuiWidget* parent) {
    if(mParent != nullptr && parent != nullptr) {
        // move ourselves to another widget
        boost::ptr_map<QString, GuiWidget>& from = mParent->GetChildrenMap();
        boost::ptr_map<QString, GuiWidget>& to = parent->GetChildrenMap();
        to.transfer(to.end(), from.find(mName), from);
    }
    if(mParent != nullptr && parent == nullptr) {
        // just remove ourselves
        mParent->GetChildrenMap().erase(mName);
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
    QStringList split = name.split('.', QString::SkipEmptyParts);
    if(split.size() == 0)
        return nullptr;

    boost::ptr_map<QString, GuiWidget>::iterator iter = mChildren.find(split.first());
    if(iter == mChildren.end()) {
        return nullptr;
    }
    if(split.size() == 1) {
        return iter->second;
    } else {
        split.removeFirst(); // remove first item from path
        QString path(split.join("."));
        return iter->second->FindChild(path);
    }
}


QScriptValue GuiWidget::GetChild(const QString& name) {
    GuiWidget* widget = FindChild(name);
    if(widget == nullptr) {
        return QScriptValue(dt::ScriptManager::Get()->GetScriptEngine(), QScriptValue::UndefinedValue);
    }
    return dt::ScriptManager::Get()->GetScriptEngine()->newQObject(widget);
}

void GuiWidget::Show() {
    if(IsVisible() != true) {
        SetVisible(true);
    }
}

void GuiWidget::Hide() {
    if(IsVisible() != false) {
        SetVisible(false);
    }
}

void GuiWidget::SetVisible(bool visible) {
    if(IsVisible() != visible) {
        mIsVisible = visible;
        GetMyGUIWidget()->setVisible(visible);
        emit VisibilityChanged(visible);
    }
}

bool GuiWidget::IsVisible() const {
    return mIsVisible;
}

void GuiWidget::RemoveChild(const QString& name) {
    GuiWidget* w = FindChild(name);
    if(w != nullptr) {
        w->Destroy();
    }
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
