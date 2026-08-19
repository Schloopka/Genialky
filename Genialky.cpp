

#include "server/server.h"
#include "server/client.h"
#include "game/Input/menu.h"

int main()
{
    const std::optional<GameMode> selectedMode = MainMenu().loop();

    if (!selectedMode.has_value()) {
        return 0;
    }

    Server server;

    if (*selectedMode == GameMode::Singleplayer) {
        SinglePlayerClient playerOne(server);
        playerOne.setup();
        while (playerOne.is_open()) {
            playerOne.process_input();
            playerOne.render();
        }
    }
    /*else {
        MultiPlayerClient playerOne(server, true);
        MultiPlayerClient playerTwo(server, false);
        playerOne.setup();
        playerTwo.setup();

        while (playerOne.is_open() && playerTwo.is_open()) {
            playerOne.process_input();
            playerTwo.process_input();

            playerOne.render();
            playerTwo.render();
        }
    }*/
    

    return 0;
    
}
