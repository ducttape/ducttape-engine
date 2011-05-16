#ifndef DUCTTAPE_ENGINE_ROOT
#define DUCTTAPE_ENGINE_ROOT

#include <boost/serialization/singleton.hpp>

#include "utils/StringManager.hpp"
#include "utils/LogManager.hpp"

namespace dt {

class Root : public boost::serialization::singleton<Root> {
public:
    Root();
    ~Root();

    StringManager* GetStringManager();
    LogManager* GetLogManager();
private:
    StringManager* mStringManager;
    LogManager* mLogManager;
};

}

#endif
