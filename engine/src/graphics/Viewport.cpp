#include "graphics/Viewport.hpp"

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

void Viewport::setCamera(Ogre::Camera* cam) {
    mViewport->setCamera(cam);
}

void Viewport::setBackgroundColour(const Ogre::ColourValue& color) {
    mViewport->setBackgroundColour(color);
}

void Viewport::hide() {
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

void Viewport::show() {
    if(hidden) {
        mViewport->setAutoUpdated(true);
        mViewport->setDimensions(left, top, width, height);
        mViewport->_updateDimensions();
        hidden = false;
    }
}

}

