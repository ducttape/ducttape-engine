#ifndef DUCTTAPE_ENGINE_TESTS_TEST
#define DUCTTAPE_ENGINE_TESTS_TEST

#include <QString>

class Test {
public:
    virtual QString GetTestName() = 0;
    virtual bool Run(int argc, char** argv) = 0;
};

#endif
