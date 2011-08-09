
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIWIDGET
#define DUCTTAPE_ENGINE_GUI_GUIWIDGET

#include <Config.hpp>

#include <boost/ptr_container/ptr_map.hpp>

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
    Q_PROPERTY(QString name READ GetName FINAL)
    Q_PROPERTY(QScriptValue parent READ GetScriptParent WRITE SetScriptParent)
    Q_PROPERTY(bool visible READ IsVisible WRITE SetVisible)

public:
    /**
      * Constructor.
      */
    GuiWidget(const QString& name);

    /**
      * Pure virtual destructor.
      */
    virtual ~GuiWidget() = 0;

    void Create();

    void Destroy();

    /**
      * Called when the widget is being created.
      */
    virtual void OnCreate() = 0;

    /**
      * Returns the MyGUI widget of this widget.
      * @returns The MyGUI widget of this widget.
      */
    virtual MyGUI::Widget* GetMyGUIWidget() = 0;

    /**
      * Returns the name of this widget.
      * @returns The name of this widget.
      */
    const QString& GetName() const;

    void Focus();

    void SetPosition(float x, float y);
    void SetPosition(int x, int y);

    void SetSize(float width, float height);
    void SetSize(int width, int height);

    void SetParent(GuiWidget* parent);
    GuiWidget* GetParent();
    QScriptValue GetScriptParent();
    void SetScriptParent(QScriptValue parent);

    /**
      * Returns a child widget.
      * @param name The name of the child widget.
      * @returns The widget pointer, or \c nullptr if no widget named \c name exists.
      */
    GuiWidget* FindChild(const QString& name);

    /**
      * Creates a widget of type WidgetType and adds as a child it to this widget.
      * @param name The name of the new widget.
      * @returns A pointer to the new widget.
      */
    template <typename WidgetType>
    WidgetType* CreateChild(const QString& name) {
        _AddChild(new WidgetType(name));
        FindChild(name)->Create();
        return dynamic_cast<WidgetType*>(FindChild(name));
    }

    void RemoveChild(const QString& name);

    boost::ptr_map<QString, GuiWidget>& GetChildrenMap();

    /**
      * Returns the full name of this widget (e.g. "RootWidgetName.FirstChildWidgetName.ThisWidgetName")
      * @returns The full name of this widget.
      */
    QString GetFullName();

public slots:
    /**
      * Returns a child widget, for scripting access.
      * @param name The name of the child widget.
      * @returns The widget, or \c UndefineValue if not found.
      */
    QScriptValue GetChild(const QString& name);

    void Show();
    void Hide();
    void SetVisible(bool visible);
    bool IsVisible() const;

private:
    /**
      * Adds a widget as a child. The widget is being owned.
      * @param widget The widget to add.
      * @returns True, if successful. False, if a widget with the name already exists.
      */
    bool _AddChild(GuiWidget* widget);

    boost::ptr_map<QString, GuiWidget> mChildren;   //!< The map of child widgets (name -> widget).

    QString mName;
    GuiWidget* mParent;
    bool mIsVisible;

};

}

#endif
