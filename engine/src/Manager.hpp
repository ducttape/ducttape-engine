#ifndef DUCTTAPE_ENGINE_MANAGER
#define DUCTTAPE_ENGINE_MANAGER

namespace dt {

class Manager {
public:
    virtual ~Manager() = 0;
    virtual void Initialize() = 0;
    virtual void Deinitialize() = 0;

    // remember to implement the "static Manager_T Get();" shortcut

};

}

#endif
