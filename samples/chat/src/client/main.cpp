#include <iostream>
#include <ctime>

#include "Client.hpp"

int main(int argc, char** argv) {
    Client client;
    if(argc > 1)
        client.SetNick(argv[1]);
    else
        client.SetNick("chatter-" + dt::tostr(time(0)));

    if(argc > 2) {
        client.SetServerIP(sf::IpAddress(argv[2]));
    }
    client.Run();
    return 0;
}
