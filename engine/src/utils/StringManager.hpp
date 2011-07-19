
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_STRINGMANAGER
#define DUCTTAPE_ENGINE_UTILS_STRINGMANAGER

#include <cstdint>
#include <string>
#include <map>

namespace dt {

/**
  * A class used to translate strings to global IDs and back. Used in server and client applications. The IDs are required to keep
  * the network traffic to a minimum (a uint32_t is smaller than most strings), but strings are used to keep the source code clean
  * and readable and to output (debug) messages in a readable format.
  */
class StringManager {
public:
    /**
      * Default constructor.
      */
    StringManager();

    /**
      * Registers a new string with a generated ID.
      * @param string the string to register
      * @returns the new ID
      */
    uint32_t Add(const std::string& string);

    /**
      * Checks whether a string is already registered.
      * @param string the string to be checked
      * @returns true if the string is already registered, otherwise false
      */
    bool Has(const std::string& string);

    /**
      * Checks whether an ID is already used.
      * @param id the ID to be checked
      * @returns true if the ID is already used, otherwise false
      */
    bool Has(uint32_t id);

    /**
      * Returns the ID for a string.
      * @param string the string to find
      * @returns the ID for the string
      */
    uint32_t Get(const std::string& string);

    /**
      * Returns the string for an ID.
      * @param id the ID to find
      * @returns the string for the ID
      */
    const std::string& Get(uint32_t id);

    /**
      * Returns next ID that is available.
      * @returns the uint32_t with next available ID
      */
    uint32_t GetNextId();

private:
    uint32_t mLastId;                       //!< the ID used to register the last string with
    std::map<uint32_t, std::string> mIds;   //!< the list of IDs/strings
};

}

#endif
