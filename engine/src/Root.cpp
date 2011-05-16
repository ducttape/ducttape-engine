#include "Root.hpp"

namespace dt {

Root::Root() {
    // -- create instances of managers etc here
    mLogManager = new LogManager();
    mStringManager = new StringManager();
}

Root::~Root() {
    // -- delete in reverse order
    delete mStringManager;
    delete mLogManager;
}

StringManager* Root::GetStringManager() {
    return mStringManager;
}

LogManager* Root::GetLogManager() {
    return mLogManager;
}

}
