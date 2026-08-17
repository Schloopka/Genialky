#include "server.h"


Server::Server():game(){

}

void Server::handle_events(GameEventContext& context){
    if (context.action_type == GameActionType::SQUARE_MOVE){
        game.handle_events(context);
    } else if(context.action_type == GameActionType::END_TURN_MOVE){
        game.try_end_turn(context);
    }
    
}
Game& Server::get_game(){
    return game;
}
