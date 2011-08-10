#ifndef DUCTTAPE_ENGINE_TESTS_TESTFRAMEWORK
#define DUCTTAPE_ENGINE_TESTS_TESTFRAMEWORK

#include "Test.hpp"

#include <QString>

#include <boost/ptr_container/ptr_map.hpp>

boost::ptr_map<QString, Test> Tests;

void AddTest(Test* test);

Test* GetTest(QString name);

int main(int argc, char** argv);

#endif
