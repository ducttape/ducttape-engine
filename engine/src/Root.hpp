/**
  * \mainpage
  * \section About
  * Welcome to the documentation for the Ducttape Game Engine - a universal open-source game engine.
  * \section License
  * Ducttape itself is licensed under the Lesser GNU Public License version 3.0 (LGPL3). However, it directly and indirectly uses many libraries and media files with different licenses. For a complete list including related license files, please see docs/licenses/.
  * \section Goals
  * \li Compatibility
  * \li Maintainability
  * \li Usability
  * \li Performance
  * \li Features
  * \li Extensibility
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

#include "scene/ComponentsManager.hpp"
#include "utils/StringManager.hpp"
#include "utils/LogManager.hpp"
#include "states/StateManager.hpp"
#include "event/EventManager.hpp"
#include "network/NetworkManager.hpp"
#include "graphics/DisplayManager.hpp"
#include "resources/ResourceManager.hpp"

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
      * Initializes all managers.
      */
    void Initialize();

    /**
      * Deinitializes all managers.
      */
    void Deinitialize();

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

    /**
      * Returns the DisplayManager.
      * @returns the DisplayManager
      */
    DisplayManager* GetDisplayManager();

    /**
      * Returns the ComponentsManager.
      * @returns the ComponentsManager
      */
    ComponentsManager* GetComponentsManager();

    /**
      * Returns the ResourceManager.
      * @returns the ResourceManager
      */
    ResourceManager* GetResourceManager();

private:
    StringManager* mStringManager;  //!< pointer to the StringManager
    LogManager* mLogManager;        //!< pointer to the LogManager
    ComponentsManager* mComponentsManager;  //!< pointer to the ComponentsManager
    StateManager* mStateManager;    //!< pointer to the StateManager
    EventManager* mEventManager;    //!< pointer to the EventManager
    NetworkManager* mNetworkManager;    //!< pointer to the NetworkManager
    DisplayManager* mDisplayManager;    //!< pointer to the DisplayManager
    ResourceManager* mResourceManager;  //!< pointer to the ResourceManager
};

}

#endif
