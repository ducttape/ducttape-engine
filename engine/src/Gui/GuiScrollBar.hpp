
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUISCROLLBAR
#define DUCTTAPE_ENGINE_GUI_GUISCROLLBAR

#include <Config.hpp>

#include <Gui/GuiWidget.hpp>

#include <MyGUI.h>

namespace dt {

/**
* GUI ProgressBar widget.
*/
class DUCTTAPE_API GuiScrollBar : public GuiWidget {
    Q_OBJECT
public:
    /**
    * Constructor.
    */
    GuiScrollBar(const QString name);

    /**
    * getMyGUIWidget.
    */
    MyGUI::Widget* getMyGUIWidget();

    /**
    * Initialize.
    */
    void onInitialize();


    /** 
      *Get scroll range 
      *@returns scroll range
      */
    size_t getScrollRange();


    /**
      *Get scroll position (value from 0 to range - 1) 
      *@returns scroll position
      */
    size_t getScrollPosition();

public slots:
    /** 
      *Set scroll range 
      *@param range scroll range
      */
    void setScrollRange(size_t range);

    /** 
      *Set scroll position (value from 0 to range - 1) 
      *@param position scroll position
      */
    void setScrollPosition(size_t position);

signals:
    void scrollRangeChanged(size_t range);
	void scrollPositionChanged(size_t position);

private:
    MyGUI::ScrollBar* mScrollBar;

};

}

#endif
