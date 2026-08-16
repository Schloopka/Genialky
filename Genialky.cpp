

#include "server/server.h"
#include "server/client.h"

int main()
{
    
    Server server;

    Client playerOne(server, true);
    Client playerTwo(server, false);

    playerOne.setup();
    playerTwo.setup();

    while (playerOne.is_open() || playerTwo.is_open()) {
        playerOne.process_input();
        playerTwo.process_input();

        playerOne.render();
        playerTwo.render();
    }

    return 0;
    
}


