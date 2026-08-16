#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "game/game.h"

class Server {
public:
    Server();
    Game& get_game();
private:
    Game game;
    bool running = true;

};

#endif
