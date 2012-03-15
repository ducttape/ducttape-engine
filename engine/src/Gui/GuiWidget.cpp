
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

GuiWidget::GuiWidget(const QString name)
    : mName(name),
      mParent(nullptr),
      mIsVisible(true) {

    if(mName.contains('.')) {
        Logger::get().warning("GuiWidget name cannot contain '.': \"" + mName + "\". All occurrences will be stripped.");
        mName = mName.replace('.', "");
        Logger::get().info("New widget name: \"" + mName + "\".");
    }
}

GuiWidget::~GuiWidget() {}

void GuiWidget::initialize() {
    GuiManager::get()->initialize(); // initialize if not already happened
    onInitialize();
}

void GuiWidget::deinitialize() {
    // destroy all children
    /*for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
        iter->second->Deinitialize();

        iter = mChildren.erase(iter);
    }*/
    removeAllChildren();
    // destroy the mygui widget
    GuiManager::get()->getGuiSystem()->destroyWidget(getMyGUIWidget());
}

const QString GuiWidget::getName() const {
    return mName;
}

void GuiWidget::focus() {
    if(MyGUI::InputManager::getInstance().getKeyFocusWidget() != getMyGUIWidget()) {
        MyGUI::InputManager::getInstance().setKeyFocusWidget(getMyGUIWidget());
        emit focused();
    }
}

void GuiWidget::setPosition(float x, float y) {
    if(getMyGUIWidget()->getAbsoluteLeft() != (int)x || getMyGUIWidget()->getAbsoluteTop() != (int)y) {
        getMyGUIWidget()->setRealPosition(x, y);
        emit positionChanged(x, y);
    }
}

void GuiWidget::setPosition(int x, int y) {
    if(getMyGUIWidget()->getAbsoluteLeft() != x || getMyGUIWidget()->getAbsoluteTop() != y) {
        getMyGUIWidget()->setPosition(x, y);
        emit positionChanged(x, y);
    }
}

void GuiWidget::setSize(float width, float height) {
    //DO NOT ADD THE TEST BELOW! OR YOU WILL NEVER SEE THE GUI!
    //if(GetMyGUIWidget()->getSize().width != width || GetMyGUIWidget()->getSize().height != height) {
        getMyGUIWidget()->setRealSize(width, height);
        emit sizeChanged(width, height);
    //}
}

void GuiWidget::setSize(int width, int height) {
    if(getMyGUIWidget()->getSize().width != width || getMyGUIWidget()->getSize().height != height) {
        getMyGUIWidget()->setSize(width, height);
        emit sizeChanged(width, height);
    }
}

// Parent management

void GuiWidget::setParent(GuiWidget* parent) {
    if(mParent != nullptr && parent != nullptr) {
        // move ourselves to another widget
        std::map<QString, GuiWidgetSP>& from = mParent->getChildrenMap();
        std::map<QString, GuiWidgetSP>& to = parent->getChildrenMap();
        GuiWidgetSP& this_widget_sp = from.find(mName)->second;
        to.insert(std::make_pair(mName, this_widget_sp));
        from.erase(mName);
    }
    if(mParent != nullptr && parent == nullptr) {
        // just remove ourselves
        mParent->getChildrenMap().erase(mName);
    }
    mParent = parent;
}

GuiWidget* GuiWidget::getParent() {
    if(mParent == nullptr) {
        // uh oh!
        Logger::get().error("Parent of widget " + getFullName() + " is nullptr.");
    }
    return mParent;
}

QScriptValue GuiWidget::getScriptParent() {
    return dt::ScriptManager::get()->getScriptEngine()->newQObject(mParent);
}

void GuiWidget::setScriptParent(QScriptValue parent) {
    QObject* q = parent.toQObject();
    mParent = dynamic_cast<GuiWidget*>(q);
}

// Children management

GuiWidget::GuiWidgetSP GuiWidget::findChild(const QString name) {
    QStringList split = name.split('.', QString::SkipEmptyParts);
    if(split.size() == 0)
        return nullptr;

    std::map<QString, GuiWidgetSP>::iterator iter = mChildren.find(split.first());
    if(iter == mChildren.end()) {
        return GuiWidgetSP();
    }
    if(split.size() == 1) {
        return iter->second;
    } else {
        split.removeFirst(); // remove first item from path
        QString path(split.join("."));
        return iter->second->findChild(path);
    }
}


QScriptValue GuiWidget::getChild(const QString name) {
    GuiWidget* widget = findChild(name).get();
    if(widget == nullptr) {
        return QScriptValue(dt::ScriptManager::get()->getScriptEngine(), QScriptValue::UndefinedValue);
    }
    return dt::ScriptManager::get()->getScriptEngine()->newQObject(widget);
}

void GuiWidget::show() {
    if(isVisible() != true) {
        setVisible(true);
    }
}

void GuiWidget::hide() {
    if(isVisible() != false) {
        setVisible(false);
    }
}

void GuiWidget::setVisible(bool visible) {
    if(isVisible() != visible) {
        mIsVisible = visible;
        getMyGUIWidget()->setVisible(visible);
        emit visibilityChanged(visible);
    }
}

bool GuiWidget::isVisible() const {
    return mIsVisible;
}

void GuiWidget::removeChild(const QString name) {
    GuiWidget* w = findChild(name).get();
    
    if(w != nullptr) {
        w->deinitialize();
        mChildren.erase(name);
    }
}

void GuiWidget::removeAllChildren() {
    for(auto child = mChildren.begin(); child != mChildren.end(); child++) {
        auto name = child->first;
        child--;
        removeChild(name);
    }
}

std::map<QString, GuiWidget::GuiWidgetSP>& GuiWidget::getChildrenMap() {
    return mChildren;
}

bool GuiWidget::_addChild(GuiWidget* widget) {
    QString name(widget->getName()); // need a copy for boost::ptr_map::insert()

    if(findChild(name) != nullptr) {
        // widget already exists
        Logger::get().error("Cannot add widget \"" + widget->getName() + "\" to \"" +
                            getFullName() + "\": widget with this name already exists.");
        return false;
    }

    GuiWidgetSP widget_sp(widget);
    mChildren.insert(std::make_pair(name, widget_sp));
    findChild(name)->setParent(this);
    return true;
}

QString GuiWidget::getFullName() {
    if(mParent == nullptr)
        return mName;
    else
        return mParent->getFullName() + "." + mName;
}

}
