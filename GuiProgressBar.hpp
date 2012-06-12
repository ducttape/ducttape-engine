
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIPROGRESSBAR
#define DUCTTAPE_ENGINE_GUI_GUIPROGRESSBAR

#include <Config.hpp>

#include <Gui/GuiWidget.hpp>

#include <MyGUI.h>

namespace dt {

/**
  * GUI ProgressBar widget.
  */
class DUCTTAPE_API GuiProgressBar : public GuiWidget {
    Q_OBJECT
public:
    /**
      * Constructor.
      */
    GuiProgressBar(const QString name);

	/**
      * getMyGUIWidget.
      */
    MyGUI::Widget* getMyGUIWidget();

	/**
      * Initialize.
      */
    void onInitialize();

	/**
	  * Get progress range 
	  *@returns Progress Range.
	  */
	size_t getProgressRange();

	/**
	  * Get progress position 
	  *@returns progress position.
	  */
	size_t getProgressPosition();

	/**
	  * Get progress auto tracking flag 
	  *@returns value.
	  */
	bool getProgressAutoTrack();

public slots:
    /**
	  * Set progress range 
	  *@param range Progress Range.
	  */
	void setProgressRange(size_t range);

	/**
	  * Set progress position 
	  *@param position progress position.
	  */
	void setProgressPosition(size_t position);

	/**
	  * Enable or disable progress auto tracking 
	  *@param _value value.
	  */
	void setProgressAutoTrack(bool _value);

signals:
    void progressRangeChanged(size_t range);
	void progressPositionChanged(size_t position);
	void progressAutoTrackChanged(bool _value);

private:
	MyGUI::ProgressBar* mProgressBar;

};

}

#endif
