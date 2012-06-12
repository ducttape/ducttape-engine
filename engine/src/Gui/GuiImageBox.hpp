
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GUI_GUIIMAGEBOX
#define DUCTTAPE_ENGINE_GUI_GUIIMAGEBOX

#include <Config.hpp>

#include <Gui/GuiWidget.hpp>

#include <MyGUI.h>

namespace dt {

/**
  * GUI ImageBox widget.
  */
class DUCTTAPE_API GuiImageBox : public GuiWidget {
    Q_OBJECT
public:
    /**
      * Constructor.
      */
    GuiImageBox(const QString name);

	 /**
      * Pure virtual destructor.
      */
    virtual ~GuiImageBox() = 0;

    /**
      * Gets the ImageTexture of the widget.
      * @returns The ImageTexture.
      */
    size_t getImageTexture();
    MyGUI::Widget* getMyGUIWidget();
    void onInitialize();

public slots:
    /**
      * Sets the ImageTexture of the widget.
      * @param caption The new ImageTexture.
      */
    void setImageTexture (const QString &texture);

signals:
    void imageTextureChanged(QString texture);

private:
	MyGUI::ImageBox* mImageBox;

};

}

#endif
