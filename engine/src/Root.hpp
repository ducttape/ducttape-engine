#ifndef DUCTTAPE_ENGINE_ROOT
#define DUCTTAPE_ENGINE_ROOT

#include <boost/serialization/singleton.hpp>

namespace dt {

class Root : public boost::serialization::singleton<Root> {
public:
    Root();
};

}

#endif
