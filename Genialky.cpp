

#include "server/server.h"
#include "server/client.h"
#include "game/Input/menu.h"

int main()
{
    const std::optional<GameMode> selectedMode = MainMenu().show();

    if (!selectedMode.has_value()) {
        return 0;
    }

    Server server;

    if (*selectedMode == GameMode::Singleplayer) {
        Client playerOne(server, true);
        playerOne.setup();
        while (playerOne.is_open()) {
            playerOne.process_input();
            playerOne.render();
        }

        return 0;
    }
    else {
        Client playerOne(server, false, true);
        Client playerTwo(server, false, false);
        playerOne.setup();
        playerTwo.setup();

        while (playerOne.is_open() && playerTwo.is_open()) {
            playerOne.process_input();
            playerTwo.process_input();

            playerOne.render();
            playerTwo.render();
    }
    }
    

    return 0;
    
}
