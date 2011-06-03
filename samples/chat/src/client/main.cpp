#include <iostream>
#include <ctime>

#include "Client.hpp"

int main(int argc, char** argv) {
    Client client;
    if(argc > 0)
        client.SetNick(argv[1]);
    else
        client.SetNick("chatter-" + dt::tostr(time(0)));
    client.Run();
    return 0;
}
