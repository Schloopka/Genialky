#pragma once
#ifndef GAME_H
#define GAME_H


#include <SFML/Window.hpp>

#include "gamestate.h"

#include "./Pieces/piece.h"
#include "./Pieces/pawn.h"
#include "./Pieces/bishop.h"
#include "./Pieces/knight.h"
#include "./Pieces/king.h"
#include "./Pieces/rook.h"
#include "./Pieces/queen.h"
#include "./Input/menu.h"
#include "Input/game_event_context.h"
#include <memory>


class Menu;
class Button;
class Piece;


class Game {
public:
	Game();
	~Game();

	void run(); //mainloop of the game
	void setup(); //calls functions to set up game, pieces and render background with fields for texts
	void setup_pieces(); //sets up pieces and puts them into a vector
	
	void process_input();

	void handle_events(GameEventContext& context); /*looks at vector of last_clicked buttons and depending on current stats calls appropriate function which will handle the case
	or does nothing when no case can be executed
	If no case is executed and length of vector is more then one, erases first button out of the vector (this allows the user to missclick, then click two buttons 
	that can interact with each other and they don't have to care about parity of number of buttons clicked*/
	void handle_normal_moves(GameEventContext& context); //handles normal moves when two squares are clicked
	void handle_queen_select_airstrike(bool white_on_move, GameEventContext& context); //handles events if the player must choose where queen lands after airstrike
	void handle_bishop_abiltiy(bool white_on_move, Piece* bishop, GameEventContext& context);//handles events when bishops ability is activated
	void try_end_turn(GameEventContext& context); //handles other buttons such as ending turn

	bool eliminate_pieces_from(int dest_row, int dest_column, attackType attack_type);/*eliminate piece from a square, also checks, if the piece can be eliminated
	returns true if any piece was eliminated (pieces can be immune to attack)*/
	bool is_there_a_piece(int row, int column); //returns true if there is a piece on that square
	bool player_has_only_one_piece_left(bool is_player_white);//returns true if there is only one piece left for the player in argument

	void update_stats(); /*lowers every piece of player who is on move(taken from gamestate) their stun, root or reload lowered by one(in this order)
	also lowers every pieces poison by one because it is stored in halfmoves*/
	void switchGamestate(GameEventContext& context); /*kills all pieces which should die of poison, change the gamestate and inputmode of players acordingly to situation in game
	changes the texts displayed*/
	void move_queen_back_to_board(bool white_on_move); //move the queen from ability square to board based on queen airstrike data
	void handle_queen_after_landing(GameEventContext& context); /*handles moves after the queen landed, if user makes move with a queen, it is done and user has one more move
	else makes move with other piece but the user doesnt get other free move*/
	void promote_piece(PieceType piece_type);
	void piece_after_menu_attack(MenuOption menu_option);
	
	void set_pieces_can_move(std::vector<Piece*> pieces);
	void set_all_pieces_can_move();
	std::vector<Piece*> get_pieces_can_move();

	void set_input_mode(ON_TURN gamestate, InputMode input_mode);//depending on gamestate, sets the input mode for that player
	InputMode get_input_mode(bool is_white); //returns input mode of player in the argument
	void set_king_on_board(bool king_on_board, bool is_white); //changes if king of a colour is on board
	bool get_king_on_board(bool is_white);

	void set_piece_was_moved_this_turn(); //switches piece was moved to true
	bool get_piece_was_moved_this_turn(); 

	int get_moves_left();
	void set_moves_left(int num); 
	std::pair<int, int> get_after_menu_coordinates();

	ON_TURN get_gamestate() const;
	std::vector<Piece*> get_pieces() const;
	void create_possible_actions(GameEventContext& context);

private:
	bool running = true;
	std::vector<std::unique_ptr<Piece>> pieces; //vector of all pieces on board
	std::vector<Piece*> pieces_can_move; //pieces that can move this turn, used when there is restriction which pieces can move
	
	ON_TURN gamestate = ON_TURN::WHITE_TURN;
	InputMode white_input_mode = InputMode::NORMAL;
	InputMode black_input_mode = InputMode::NORMAL;

	std::pair<int, int> after_menu_dest_coordinates = { 0, 0 }; //pair to remember which square is attacked by a piece when menu is needed after attack
	Piece* menu_resolving_piece = nullptr; //piece which the current menu is displayed about, ie piece which will be promoted
	
	bool white_king_on_board = true;
	bool black_king_on_board = true;
	bool piece_was_moved_this_turn = false; //literally if any piece moved from one square to another, used so rook can use its passive ability
	int moves_left = 1; //number of moves the player has until the gamestate changes and the opponent moves
};

#endif
