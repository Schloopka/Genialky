#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "game/game.h"

class Server {
public:
    Server();
    void handle_events(GameEventContext& context);
    RenderContext get_render_context() const;
    Game& get_game();
private:
    Game game;
    bool running = true;

};

#endif
