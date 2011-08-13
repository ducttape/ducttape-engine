
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

/**
  * \mainpage
  * \section About
  * Welcome to the documentation for the Ducttape Engine - a universal free and open-source game engine
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
  * \li Thomas \a 'tomt610' Fiedor
  * \li Antonio \a 'amalvagomes' MalvaGomes
  * \subsection Media
  * \li Darren \a 'visionseeker' Cullen (http://www.deliciousmusic.co.nz/artists/visionseeker)
  * \li Lasse \a 'lazzly' Bienkowski
  */

#ifndef DUCTTAPE_ENGINE_ROOT
#define DUCTTAPE_ENGINE_ROOT

#include <Config.hpp>

#include <boost/noncopyable.hpp>

#include <SFML/System/Clock.hpp>

#include <QCoreApplication>

namespace dt {

class LogManager;
class StringManager;
class EventManager;
class ResourceManager;
class InputManager;
class DisplayManager;
class StateManager;
class NetworkManager;
class PhysicsManager;
class TerrainManager;
class ScriptManager;

/**
  * Engine Root class holding various Manager instances. This class is designed to be the only singleton in the whole engine,
  * so the creation order can be controlled.
  * @see StringManager
  * @see LogManager
  * @see StateManager
  */
class DUCTTAPE_API Root : public boost::noncopyable {
public:
    static QString _VERSION;  //!< Metadata: engine version number (Format: "0.0.0")

    /**
      * Destructor. All instances are deleted here.
      */
    ~Root();

    /**
     * Gets singleton instance reference.
     * @returns singleton instance reference to itself
     */
    static Root& GetInstance();

    /**
      * Initializes all managers.
      */
    void Initialize(int argc, char** argv);

    /**
      * Deinitializes all managers.
      */
    void Deinitialize();

    /**
      * Gets time since calling Initialize().
      * @returns The time in seconds since calling Initialize()
      */
    double GetTimeSinceInitialize() const;

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
      * Returns the InputManager.
      * @returns the InputManager
      */
    InputManager* GetInputManager();

    /**
      * Returns the ResourceManager.
      * @returns the ResourceManager
      */
    ResourceManager* GetResourceManager();

    /**
      * Returns the PhysicsManager.
      * @returns the PhysicsManager
      */
    PhysicsManager* GetPhysicsManager();

    /**
      * Returns the ScriptManager.
      * @returns the ScriptManager
      */
    ScriptManager* GetScriptManager();

    /**
      * Returns the TerrainManager.
      * @returns the TerrainManager
      */
    TerrainManager* GetTerrainManager();

private:
    /**
      * Private default constructor (for singleton). All instances are created here.
      */
    Root();

    sf::Clock mSfClock;                 //!< Clock for keeping time since Initialize() was called.
    QCoreApplication* mCoreApplication; //!< Pointer to the Qt Core Application (required for QScriptEngine and command line parameter parsing).

    LogManager* mLogManager;            //!< Pointer to the LogManager.
    StringManager* mStringManager;      //!< Pointer to the StringManager.
    EventManager* mEventManager;        //!< Pointer to the EventManager.
    ResourceManager* mResourceManager;  //!< Pointer to the ResourceManager.
    InputManager* mInputManager;        //!< Pointer to the InputManager.
    DisplayManager* mDisplayManager;    //!< Pointer to the DisplayManager.
    StateManager* mStateManager;        //!< Pointer to the StateManager.
    NetworkManager* mNetworkManager;    //!< Pointer to the NetworkManager.
    PhysicsManager* mPhysicsManager;    //!< Pointer to the PhysicsManager.
    TerrainManager* mTerrainManager;    //!< Pointer to the TerrainManager.
    ScriptManager* mScriptManager;      //!< Pointer to the ScriptManager;
};

}

#endif
