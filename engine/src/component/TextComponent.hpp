
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_TEXTCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_TEXTCOMPONENT

#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreOverlayContainer.h>
#include <OGRE/OgreOverlayElement.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreFontManager.h>

#include "Config.hpp"
#include "component/Component.hpp"
#include "graphics/DisplayManager.hpp"
#include "utils/Utils.hpp"

namespace dt {

/**
  * Component to add a text on the screen, at the position where the camera would see the node.
  */
class DUCTTAPE_API TextComponent : public Component {
public:
    TextComponent(const std::string& text, const std::string& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    void SetText(const std::string& text);
    const std::string& GetText() const;

    void SetFont(const std::string& fontname);
    const std::string& GetFont() const;

    void SetColor(Ogre::ColourValue color);
    Ogre::ColourValue GetColor() const;

    void SetFontSize(uint8_t font_size);
    uint8_t GetFontSize() const;

    void SetBackgroundMaterial(const std::string& material_name);
    const std::string& GetBackgroundMaterial() const;

private:
    std::string mText;
    Ogre::Overlay* mOverlay;
    Ogre::OverlayContainer* mPanel;
    Ogre::TextAreaOverlayElement* mLabel;
    bool mRefresh;
    std::string mFont;
    uint8_t mFontSize;
    Ogre::ColourValue mColor;
    float mTextWidth;
    std::string mBackgroundMaterial;

};

}

#endif
