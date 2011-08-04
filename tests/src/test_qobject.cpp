#include <Core/Root.hpp>
#include <Scene/Component.hpp>
#include <Logic/TriggerComponent.hpp>

#include <QVariant>
#include <cstdlib>

int main(int argc, char** argv) {
    // Don't initialize Root, we don't need the StringManager.
    // We gave that Component a name. Components love names.

    dt::TriggerComponent trigger("testcomponent");
    QObject *object = &trigger;
    QVariant var = object->property("mName");
    char* c = var.toString().toAscii().data();

    if(strcmp(c, "testcomponent") != 0) {
        std::cerr << "Could not get property [mName] of TriggerComponent. Got \"" << c << "\" instead of \"testcomponent\"." << std::endl;
        exit(1);
    }
    std::cout << "QObject: OK" << std::endl;
    return 0;
}
