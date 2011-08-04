
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef TEST_SIGNALS
#define TEST_SIGNALS

#include <Core/Root.hpp>

#include <QObject>

class TestComponent : public dt::Component {
    Q_OBJECT

public:
    TestComponent(const QString& name)
        : Component(name) {}

    void OnCreate() {}

    void OnDestroy() {}

public slots:
    void Invoke() {
        emit Invoked();
    }

signals:
        void Invoked();
};

class CallbackClass : public QObject {
    Q_OBJECT

public:
    CallbackClass() : invoked(false) {}
    bool invoked;

public slots:
    void the_callback() {
        std::cout << "Callback was called." << std::endl;
        invoked = true;
    }
};

#endif
