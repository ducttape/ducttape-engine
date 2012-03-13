
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_MOUSESTATE
#define DUCTTAPE_ENGINE_INPUT_MOUSESTATE

#include <Config.hpp>

#include <OIS.h>

#include <QObject>

namespace dt {

/**
  * Helper class for binding the OIS mouse state in scripts.
  */
class DUCTTAPE_API MouseState : public QObject {
    Q_OBJECT
    Q_PROPERTY(int x READ getX)
    Q_PROPERTY(int y READ getY)
    Q_PROPERTY(bool left READ isLeftButtonDown)
    Q_PROPERTY(bool right READ isRightButtonDown)
    Q_PROPERTY(bool middle READ isMiddleButtonDown)

public slots:
    bool isButtonDown(int button);
    bool isLeftButtonDown();
    bool isRightButtonDown();
    bool isMiddleButtonDown();
    int getX();
    int getY();

};

}

#endif
