
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_TEXTCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_TEXTCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <OgreColourValue.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreVector2.h>

#include <QString>

namespace dt {

/**
  * Component to add a text on the screen, at the position where the camera would see the node.
  */
class DUCTTAPE_API TextComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      */
    TextComponent(const QString& text, const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
      * Sets the text of the TextComponent.
      * @param text The new text.
      */
    void SetText(const QString& text);

    /**
      * Gets the text displayed.
      * @returns The text displayed.
      */
    const QString& GetText() const;

    /**
      * Sets the font to use.
      * @param fontname The name of the font resource.
      * @see http://www.ogre3d.org/docs/manual/manual_44.html
      */
    void SetFont(const QString& fontname);

    /**
      * Returns the font used.
      * @returns The name of the font resource.
      */
    const QString& GetFont() const;

    /**
      * Sets the font color.
      * @param color The font color.
      */
    void SetColor(Ogre::ColourValue color);

    /**
      * Returns the font color.
      * @returns The font color.
      */
    Ogre::ColourValue GetColor() const;

    /**
      * Sets the font size.
      * @param font_size The font size, in standard points.
      */
    void SetFontSize(uint8_t font_size);

    /**
      * Returns the font size.
      * @returns The font size, in standard points.
      */
    uint8_t GetFontSize() const;

    /**
      * Sets the material for the background panel.
      * @param material_name The name of the background material resource.
      */
    void SetBackgroundMaterial(const QString& material_name);

    /**
      * Returns the material of the background panel.
      * @returns The name of the background material resource.
      */
    const QString& GetBackgroundMaterial() const;

    /**
      * Sets the padding for the text box.
      * @param padding The padding (spacing between text and panel border), in pixels.
      */
    void SetPadding(Ogre::Vector2 padding);

    /**
      * Returns the padding for the text box.
      * @returns The padding, in pixels.
      */
    Ogre::Vector2 GetPadding() const;

private:
    QString mText;          //!< The text to display.
    Ogre::Overlay* mOverlay;    //!< The overlay created for this text.
    Ogre::OverlayContainer* mPanel; //!< The background panel.
    Ogre::TextAreaOverlayElement* mLabel;   //!< The overlay label.
    bool mRefresh;              //!< Whether the text width has to be recalculated.
    QString mFont;          //!< The font name for the text.
    uint8_t mFontSize;          //!< The font size.
    Ogre::ColourValue mColor;   //!< The text color.
    float mTextWidth;           //!< The width of the displayed text, in pixels.
    QString mBackgroundMaterial;    //!< The name of the background panel material.
    Ogre::Vector2 mPadding;     //!< The padding, in pixels.

};

}

#endif
