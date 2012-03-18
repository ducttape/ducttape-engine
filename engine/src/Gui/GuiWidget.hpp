
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIWIDGET
#define DUCTTAPE_ENGINE_GUI_GUIWIDGET

#include <Config.hpp>

#include <MyGUI.h>

#include <QObject>
#include <QScriptValue>
#include <QString>

namespace dt {

/**
  * Base class for all GUI widgets.
  */
class DUCTTAPE_API GuiWidget : public QObject  {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName FINAL)
    Q_PROPERTY(QScriptValue parent READ getScriptParent WRITE setScriptParent)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

public:
    
    typedef std::shared_ptr<GuiWidget> GuiWidgetSP;
    
    /**
      * Constructor.
      */
    GuiWidget(const QString name);

    /**
      * Pure virtual destructor.
      */
    virtual ~GuiWidget() = 0;

    void initialize();

    void deinitialize();

    /**
      * Called when the widget is being created.
      */
    virtual void onInitialize() = 0;

    /**
      * Returns the MyGUI widget of this widget.
      * @returns The MyGUI widget of this widget.
      */
    virtual MyGUI::Widget* getMyGUIWidget() = 0;

    /**
      * Returns the name of this widget.
      * @returns The name of this widget.
      */
    const QString getName() const;

    void setParent(GuiWidget* parent);
    GuiWidget* getParent();
    QScriptValue getScriptParent();
    void setScriptParent(QScriptValue parent);

    /**
      * Returns a child widget.
      * @param name The name of the child widget.
      * @returns The widget pointer, or \c nullptr if no widget named \c name exists.
      */
    GuiWidget::GuiWidgetSP findChild(const QString name);

    /**
      * Creates a widget of type WidgetType and adds as a child it to this widget.
      * @param name The name of the new widget.
      * @returns A pointer to the new widget.
      */
    template <typename WidgetType>
    std::shared_ptr<WidgetType> addChildWidget(WidgetType* widget) {
        const QString name = widget->getName();
        _addChild(widget);
        findChild(name)->initialize();
        return std::dynamic_pointer_cast<WidgetType>(findChild(name));
    }

    void removeChild(const QString name);

    void removeAllChildren();

    std::map<QString, GuiWidget::GuiWidgetSP>& getChildrenMap();

    /**
      * Returns the full name of this widget (e.g. "RootWidgetName.FirstChildWidgetName.ThisWidgetName")
      * @returns The full name of this widget.
      */
    QString getFullName();

public slots:
    void focus();

    void setPosition(float x, float y);
    void setPosition(int x, int y);

    void setSize(float width, float height);
    void setSize(int width, int height);

    /**
      * Returns a child widget, for scripting access.
      * @param name The name of the child widget.
      * @returns The widget, or \c UndefineValue if not found.
      */
    QScriptValue getChild(const QString name);

    void show();
    void hide();
    void setVisible(bool visible);
    bool isVisible() const;

signals:
    void focused();
    void positionChanged(float x, float y);
    void positionChanged(int x, int y);
    void sizeChanged(float width, float height);
    void sizeChanged(int width, int height);
    void shown();
    void hidden();
    void visibilityChanged(bool visible);

private:
    /**
      * Adds a widget as a child. The widget is being owned.
      * @param widget The widget to add.
      * @returns True, if successful. False, if a widget with the name already exists.
      */
    bool _addChild(GuiWidget* widget);

    std::map<QString, GuiWidgetSP> mChildren;   //!< The map of child widgets (name -> widget).

    QString mName;
    GuiWidget* mParent;
    bool mIsVisible;

};

}

#endif
