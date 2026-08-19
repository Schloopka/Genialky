#include "server.h"
#include "game/Input/menu.h"

Server::Server():game(){

}

void Server::handle_events(GameEventContext& context){
    if (context.action_type == GameActionType::SQUARE_MOVE){
        game.handle_events(context);
    } else if(context.action_type == GameActionType::END_TURN_MOVE){
        game.try_end_turn(context);
    } else if (context.action_type == GameActionType::MENU_CLICK){
        switch (context.menu_option)
        {
        case MenuOption::DONT_MOVE:
            game.piece_after_menu_attack(MenuOption::DONT_MOVE);
            context.menu.reset();
            break;

        case MenuOption::MOVE_TO_ATTACKED_SQUARE:
            game.piece_after_menu_attack(MenuOption::MOVE_TO_ATTACKED_SQUARE);
            context.menu.reset();
            break;

        case MenuOption::BISHOP:
            game.promote_piece(PieceType::BISHOP);
            break;
        case MenuOption::KNIGHT:
            game.promote_piece(PieceType::KNIGHT);
            break;
        case MenuOption::ROOK:
            game.promote_piece(PieceType::ROOK);
            break;
        case MenuOption::QUEEN:
            game.promote_piece(PieceType::QUEEN);
            break;
        }
    }
    
}
RenderContext Server::get_render_context() const {
    return {game.get_pieces(), game.get_gamestate()};
}

Game& Server::get_game() {
    return game;
}
