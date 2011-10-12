
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

TextComponent::TextComponent(const QString& text, const QString& name)
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

void TextComponent::OnCreate() {
    // overlay
    QString oname = GetNode()->GetName() + "-" + mName;
    mOverlay = Ogre::OverlayManager::getSingleton().create(Utils::ToStdString(oname) + "-overlay");

    mPanel = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", Utils::ToStdString(oname) + "-panel"));
    mPanel->setDimensions(0.0, 0.0);

    mLabel = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", Utils::ToStdString(oname) + "-label"));

    mLabel->setMetricsMode(Ogre::GMM_PIXELS);
    mLabel->setPosition(0, 0);
    SetColor(mColor);
    SetFontSize(mFontSize);
    SetFont(mFont);
    SetBackgroundMaterial(mBackgroundMaterial);
    SetText(mText);
    mRefresh = true;

    mOverlay->add2D(mPanel);
    mPanel->addChild(mLabel);
    mOverlay->show();
}

void TextComponent::OnDestroy() {
    Ogre::OverlayManager* mgr = Ogre::OverlayManager::getSingletonPtr();

    mPanel->removeChild(mLabel->getName());
    mOverlay->remove2D(mPanel);

    mgr->destroyOverlayElement(mLabel);
    mgr->destroyOverlayElement(mPanel);
    mgr->destroy(mOverlay);
}

void TextComponent::OnEnable() {
    mOverlay->show();
}

void TextComponent::OnDisable() {
    mOverlay->hide();
}

void TextComponent::OnUpdate(double time_diff) {
    if(mRefresh && mFont != "") {
        // calculate the text width
        mTextWidth = 0;
        Ogre::Font* font = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingleton().getByName(Utils::ToStdString(mFont)).getPointer());
        if(font == nullptr) {
            Logger::Get().Warning("Cannot find font: \"" + mFont + "\".");
        } else {
            std::string str = Utils::ToStdString(mText);
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

    if(DisplayManager::Get()->GetMainCamera() == nullptr) {
        Logger::Get().Error("Cannot get main camera for text component: no main camera set. Disabling text component " + mName + ".");
        Disable();
        return;
    }
    Ogre::Camera* camera = DisplayManager::Get()->GetMainCamera()->GetCamera();
    Ogre::Vector3 screen_pos(camera->getProjectionMatrix() * camera->getViewMatrix() * GetNode()->GetPosition(Node::SCENE));

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

void TextComponent::SetText(const QString& text) {
        mText = text;
        if(mLabel != nullptr) {
            mLabel->setCaption(Utils::ToStdString(mText));
            mRefresh = true;
        }
        emit TextChanged();
}

const QString& TextComponent::GetText() const {
    return mText;
}

void TextComponent::SetFont(const QString& fontname) {
    mFont = fontname;
    if(mLabel != nullptr && mFont != "") {
        mLabel->setFontName(Utils::ToStdString(mFont));
    }
}

const QString& TextComponent::GetFont() const {
    return mFont;
}

void TextComponent::SetColor(Ogre::ColourValue color) {
        mColor = color;
        if(mLabel != nullptr) {
            mLabel->setColour(mColor);
        }
        emit ColorChanged();
}

Ogre::ColourValue TextComponent::GetColor() const {
    return mColor;
}

void TextComponent::SetFontSize(uint8_t font_size) {
    mFontSize = font_size;
    if(mLabel != nullptr) {
        mLabel->setCharHeight(mFontSize);
        mRefresh = true;
    }
}

uint8_t TextComponent::GetFontSize() const {
    return mFontSize;
}

void TextComponent::SetBackgroundMaterial(const QString& material_name) {
    mBackgroundMaterial = material_name;
    if(mPanel != nullptr && mBackgroundMaterial != "") {
        mPanel->setMaterialName(Utils::ToStdString(mBackgroundMaterial));
    }
}

const QString& TextComponent::GetBackgroundMaterial() const {
    return mBackgroundMaterial;
}

void TextComponent::SetPadding(Ogre::Vector2 padding) {
    mPadding = padding;
}

Ogre::Vector2 TextComponent::GetPadding() const {
    return mPadding;
}

}
