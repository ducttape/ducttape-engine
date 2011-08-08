
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_KEYBOARDSTATE
#define DUCTTAPE_ENGINE_INPUT_KEYBOARDSTATE

#include <Config.hpp>

#include <OIS.h>

#include <QObject>

namespace dt {

/**
  * Helper class for binding the OIS keyboard state in scripts.
  */
class DUCTTAPE_API KeyboardState : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool shift READ IsShiftDown)
    Q_PROPERTY(bool alt READ IsAltDown)
    Q_PROPERTY(bool ctrl READ IsCtrlDown)

public slots:
    bool IsKeyDown(int key_code) const;
    bool IsAltDown() const;
    bool IsShiftDown() const;
    bool IsCtrlDown() const;

};

}

#endif
