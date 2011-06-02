#ifndef DUCTTAPE_ENGINE_NETWORK_CONNECTION
#define DUCTTAPE_ENGINE_NETWORK_CONNECTION

#include <SFML/Network.hpp>

namespace dt {

/**
  * Structure holding information for one connection. A connection consists of an IP and a port and represents
  * a remote device (i.e. a client in server mode and vice versa).
  */
class Connection {
public:
    /**
      * Default constructor. Needed for boost::ptr_map.
      */
    Connection();

    /**
      * Advanced constructor.
      * @param address The IPAddress to use.
      * @param port The port number to use.
      */
    Connection(sf::IpAddress address, uint16_t port);

    /**
      * Destructor.
      */
    ~Connection();

    /**
      * Setter for the IPAddress.
      * @param address The IPAddress to use.
      */
    void SetIPAddress(sf::IpAddress address);

    /**
      * Getter for the IPAddress.
      * @returns The IPAddress of the remote device.
      */
    sf::IpAddress GetIPAddress() const;

    /**
      * Setter for the port number.
      * @param port The port number to use.
      */
    void SetPort(uint16_t port);

    /**
      * Getter for IPAddress.
      * @returns The port number of the remote device.
      */
    uint16_t GetPort() const;

private:
    sf::IpAddress mIPAddress;   //!< The IPAddress of the remote device.
    uint16_t mPort;             //!< The port number of the remote device.

};

}

#endif
