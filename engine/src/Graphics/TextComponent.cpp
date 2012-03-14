
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/TextComponent.hpp>

#include <Graphics/DisplayManager.hpp>
#include <Scene/Node.hpp>
#include <Utils/Utils.hpp>

#include <OgreCamera.h>
#include <OgreFontManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>

namespace dt {

TextComponent::TextComponent(const QString text, const QString name)
    : Component(name),
      mText(text),
      mOverlay(nullptr),
      mPanel(nullptr),
      mLabel(nullptr),
      mRefresh(false),
      mFont(""),
      mFontSize(12),
      mColor(Ogre::ColourValue::White),
      mTextWidth(0),
      mBackgroundMaterial(""),
      mPadding(Ogre::Vector2(10,4)) {}

void TextComponent::onInitialize() {
    // overlay
    QString oname = getNode()->getName() + "-" + mName;
    mOverlay = Ogre::OverlayManager::getSingleton().create(Utils::toStdString(oname) + "-overlay");

    mPanel = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", Utils::toStdString(oname) + "-panel"));
    mPanel->setDimensions(0.0, 0.0);

    mLabel = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", Utils::toStdString(oname) + "-label"));

    mLabel->setMetricsMode(Ogre::GMM_PIXELS);
    mLabel->setPosition(0, 0);
    setColor(mColor);
    setFontSize(mFontSize);
    setFont(mFont);
    setBackgroundMaterial(mBackgroundMaterial);
    setText(mText);
    mRefresh = true;

    mOverlay->add2D(mPanel);
    mPanel->addChild(mLabel);
    mOverlay->show();
}

void TextComponent::onDeinitialize() {
    Ogre::OverlayManager* mgr = Ogre::OverlayManager::getSingletonPtr();

    mPanel->removeChild(mLabel->getName());
    mOverlay->remove2D(mPanel);

    mgr->destroyOverlayElement(mLabel);
    mgr->destroyOverlayElement(mPanel);
    mgr->destroy(mOverlay);
}

void TextComponent::onEnable() {
    mOverlay->show();
}

void TextComponent::onDisable() {
    mOverlay->hide();
}

void TextComponent::onUpdate(double time_diff) {
    if(mRefresh && mFont != "") {
        // calculate the text width
        mTextWidth = 0;
        Ogre::Font* font = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingleton().getByName(Utils::toStdString(mFont)).getPointer());
        if(font == nullptr) {
            Logger::get().warning("Cannot find font: \"" + mFont + "\".");
        } else {
            std::string str = Utils::toStdString(mText);
            for(Ogre::String::iterator iter = str.begin(); iter < str.end(); ++iter) {
                if(*iter == 0x0020) {
                    mTextWidth += font->getGlyphAspectRatio(0x0030);
                } else {
                    mTextWidth += font->getGlyphAspectRatio(*iter);
                }
            }
            mTextWidth *= mFontSize;
        }
        mRefresh = false;
    }

    // set the position

    if(DisplayManager::get()->getMainCamera() == nullptr) {
        Logger::get().error("Cannot get main camera for text component: no main camera set. Disabling text component " + mName + ".");
        disable();
        return;
    }
    Ogre::Camera* camera = DisplayManager::get()->getMainCamera()->getCamera();
    Ogre::Vector3 screen_pos(camera->getProjectionMatrix() * camera->getViewMatrix() * getNode()->getPosition(Node::SCENE));

    if(screen_pos.z >= 1) {
        // behind or in the camera, hide
        mOverlay->hide();
    } else {
        mOverlay->show();
    }

    float x = 1.0f - ((-screen_pos.x * 0.5f) + 0.5f);   // 0 <= x <= 1 // left := 0,right := 1
    float y = ((-screen_pos.y * 0.5f) + 0.5f);          // 0 <= y <= 1 // bottom := 0,top := 1

    x *= camera->getViewport()->getActualWidth();
    y *= camera->getViewport()->getActualHeight();

    mPanel->setMetricsMode(Ogre::GMM_PIXELS);
    mPanel->setWidth(mTextWidth + 2 * mPadding.x);
    mPanel->setHeight(mFontSize + 2 * mPadding.y);
    mPanel->setLeft(x - mTextWidth / 2 - mPadding.x);
    mPanel->setTop(y - mFontSize / 2 - mPadding.y);

    mLabel->setPosition(mPadding.x, mPadding.y);
    mLabel->setDimensions(mTextWidth, mFontSize);
}

void TextComponent::setText(const QString text) {
        mText = text;
        if(mLabel != nullptr) {
            mLabel->setCaption(Utils::toStdString(mText));
            mRefresh = true;
        }
        emit textChanged();
}

const QString TextComponent::getText() const {
    return mText;
}

void TextComponent::setFont(const QString fontname) {
    mFont = fontname;
    if(mLabel != nullptr && mFont != "") {
        mLabel->setFontName(Utils::toStdString(mFont));
    }
}

const QString TextComponent::getFont() const {
    return mFont;
}

void TextComponent::setColor(Ogre::ColourValue color) {
        mColor = color;
        if(mLabel != nullptr) {
            mLabel->setColour(mColor);
        }
        emit colorChanged();
}

void TextComponent::setColor(float r, float g, float b, float a) {
    setColor(Ogre::ColourValue(r, g, b, a));
}

Ogre::ColourValue TextComponent::getColor() const {
    return mColor;
}

void TextComponent::setFontSize(uint8_t font_size) {
    mFontSize = font_size;
    if(mLabel != nullptr) {
        mLabel->setCharHeight(mFontSize);
        mRefresh = true;
    }
}

uint8_t TextComponent::getFontSize() const {
    return mFontSize;
}

void TextComponent::setBackgroundMaterial(const QString material_name) {
    mBackgroundMaterial = material_name;
    if(mPanel != nullptr && mBackgroundMaterial != "") {
        mPanel->setMaterialName(Utils::toStdString(mBackgroundMaterial));
    }
}

const QString TextComponent::getBackgroundMaterial() const {
    return mBackgroundMaterial;
}

void TextComponent::setPadding(Ogre::Vector2 padding) {
    mPadding = padding;
}

void TextComponent::setPadding(float x, float y) {
    setPadding(Ogre::Vector2(x, y));
}

Ogre::Vector2 TextComponent::getPadding() const {
    return mPadding;
}

}
