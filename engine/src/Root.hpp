/**
  * \mainpage
  * \section About
  * This is the duct tape engine. It is verry cool!
  * \section Credits
  * \subsection Programming
  * \li Paul \a 'opatut' Bienkowski
  * \li Sven-Hendrik \a 'Svenstaro' Haase
  * \li Matz-Jona \a 'Timesqueezer' Radloff
  * \li Fabian \a 'zetaron' Stegemann
  */

#ifndef DUCTTAPE_ENGINE_ROOT
#define DUCTTAPE_ENGINE_ROOT

#include <boost/serialization/singleton.hpp>

#include "utils/StringManager.hpp"
#include "utils/LogManager.hpp"
#include "states/StateManager.hpp"
#include "event/EventManager.hpp"
#include "network/NetworkManager.hpp"

namespace dt {

/**
  * Engine Root class holding various Manager instances. This class is designed to be the only singleton in the whole engine,
  * so the creation order can be controlled.
  * @see StringManager
  * @see LogManager
  * @see StateManager
  */
class Root : public boost::serialization::singleton<Root> {
public:
    /**
      * Default constructor. All instances are created here.
      */
    Root();

    /**
      * Destructor. All instances are deleted here.
      */
    ~Root();

    /**
      * Returns the StringManager.
      * @returns the StringManager
      */
    StringManager* GetStringManager();

    /**
      * Returns the LogManager.
      * @returns the LogManager
      */
    LogManager* GetLogManager();

    /**
      * Returns the StateManager.
      * @returns the StateManager
      */
    StateManager* GetStateManager();

    /**
      * Returns the EventManager.
      * @returns the EventManager
      */
    EventManager* GetEventManager();

    /**
      * Returns the NetworkManager.
      * @returns the NetworkManager
      */
    NetworkManager* GetNetworkManager();
private:
    StringManager* mStringManager;  //!< the StringManager
    LogManager* mLogManager;        //!< the LogManager
    StateManager* mStateManager;    //!< the StateManager
    EventManager* mEventManager;    //!< the EventManager
    NetworkManager* mNetworkManager;    //!< the NetworkManager
};

}

#endif
