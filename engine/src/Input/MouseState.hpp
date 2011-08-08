
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
    Q_PROPERTY(int x READ GetX)
    Q_PROPERTY(int y READ GetY)
    Q_PROPERTY(bool left READ IsLeftButtonDown)
    Q_PROPERTY(bool right READ IsRightButtonDown)
    Q_PROPERTY(bool middle READ IsMiddleButtonDown)

public slots:
    bool IsButtonDown(int button);
    bool IsLeftButtonDown();
    bool IsRightButtonDown();
    bool IsMiddleButtonDown();
    int GetX();
    int GetY();

};

}

#endif
