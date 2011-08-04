
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/Root.hpp>

class TestComponent : public dt::Component {
public:
    TestComponent(const QString& name)
        : Component(name) {}

    void OnCreate() {}

    void OnDestroy() {}

    void Invoke() {
        _CallSignal("Invoke");
    }
};

bool invoked = false;

void the_callback() {
    std::cout << "Callback was called." << std::endl;
    invoked = true;
}

int main(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    TestComponent component("test");
    component.BindSlot("Invoke", boost::bind(&the_callback));
    component.Invoke();

    if(!invoked) {
        std::cerr << "Signal callback not received correctly." << std::endl;
        exit(1);
    }

    root.Deinitialize();
    return 0;
}
