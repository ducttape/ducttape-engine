
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


namespace dt {

Viewport::Viewport() {
}

Viewport::~Viewport() {}

void Viewport::Initialize(Ogre::Viewport* Viewport) {
    mViewport = Viewport;
    hidden = false;
}

void Viewport::Deinitialize() {

}

void Viewport::SetCamera(Ogre::Camera* cam) {
    mViewport->setCamera(cam);
}

void Viewport::SetBackgroundColor(const Ogre::ColourValue& color) {
    mViewport->setBackgroundColour(color);
}

void Viewport::Hide() {
    if(!hidden) {
        mViewport->setAutoUpdated(false);
        top = mViewport->getTop();
        left = mViewport->getLeft();
        height = mViewport->getHeight();
        width = mViewport->getWidth();
        mViewport->setDimensions(0, 0, 0, 0);
        mViewport->_updateDimensions();
        hidden = true;
    }
}

void Viewport::Show() {
    if(hidden) {
        mViewport->setAutoUpdated(true);
        mViewport->setDimensions(left, top, width, height);
        mViewport->_updateDimensions();
        hidden = false;
    }
}

}

