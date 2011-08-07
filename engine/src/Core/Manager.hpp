
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_MANAGER
#define DUCTTAPE_ENGINE_MANAGER

#include <Config.hpp>

#include <boost/noncopyable.hpp>

#include <QObject>

namespace dt {

/**
  * The Manager interface class.
  * @note Remember to implement the \code static YourManager* Get(); \endcode shortcut for new managers!
  */
class DUCTTAPE_API Manager : public QObject, public boost::noncopyable {
public:
    /**
      * Pure virtual destructor.
      */
    virtual ~Manager() = 0;

    /**
      * Called when the Manager is being initialized.
      */
    virtual void Initialize() = 0;

    /**
      * Called when the Manager is being deinitialized.
      */
    virtual void Deinitialize() = 0;

};

}

#endif
