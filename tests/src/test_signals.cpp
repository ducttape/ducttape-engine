
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "test_signals.hpp"

#include <Core/Root.hpp>

#include <QObject>

namespace test_signals {

int Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    TestComponent component("test");
    CallbackClass callback;
    QObject::connect(&component, SIGNAL(Invoked()),
                     &callback, SLOT(the_callback()));
    component.Invoke();

    if(!callback.invoked) {
        std::cerr << "Signal callback not received correctly." << std::endl;
        exit(1);
    }

    root.Deinitialize();
    return 0;
}

} // namespace test_signals
