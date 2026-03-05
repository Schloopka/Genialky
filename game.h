#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include "gamestate.h"
#include <memory>
/*#include "buttons.h"
#include "piece.h"

#include "menu.h"*/

class Menu;
class Button;
class Piece;


class Game {
public:
	Game();
	~Game();

	void run();
	void setup();
	void setup_texts();
	void render_background(sf::RenderWindow& window);
	void render_pieces(sf::RenderWindow& window); //renders pieces, called after every move
	void render_buttons(sf::RenderWindow& window); //renders all the buttons to window
	void render_texts(sf::RenderWindow& window); //renders the text that describes what happened to user
	void make_buttons(sf::RenderWindow& window); //make buttons at the start of the game, put them into vector
	void setup_pieces(sf::RenderWindow& window); //sets up pieces and puts them into a vector
	void set_message_for_user(std::string message);
	void check_for_events(sf::RenderWindow& window, std::vector<Button*> buttons);//check for events and calls isClicked if button is clicked


	void handle_events(); //handles events when a button is clicked
	void handle_normal_moves(); //handles normal moves when two squares are clicked
	void handle_queen_select_airstrike(bool white_on_move); //handles events if the player must choose where queen lands after airstrike
	void handle_bishop_abiltiy(bool white_on_move, Piece* bishop);//handles events when bishops ability is activated
	void handle_other_buttons(); //handles other buttons such as ending turn
	void clear_buttons_clicked(); //clears the vector of last clicked buttons after action with them is made
	void append_buttons_clicked(Button* button);

	void eliminate_pieces_from(int dest_row, int dest_column, attackType attack_type);//eliminate piece from a square, also checks, if the piece can be eliminated
	bool isThereAPiece(int row, int column); //returns true if there is a piece on that square
	bool only_one_piece_left(bool is_color_white);//returns true if there is only one piece left

	void update_stats(); //after each turn, every piece of the person whose move ended has their stun, root or reload lowered by one (in this order)
	void switchGamestate(); //makes the opposite player move
	void switchGamestateAfterQueenAbility(bool white_on_move); //special switching gamestate after queen ability was used
	
	void promote_piece(Piece& promoting_piece, std::pair<int, int> dest_coordinates, PieceType piece_type);
	
	void set_pieces_can_move(std::vector<Piece*> pieces);
	std::vector<Piece*> get_pieces_can_move();

	void set_input_mode(Gamestate gamestate, InputMode input_mode);//depending on gamestate, sets the input mode for that player
	InputMode get_input_mode(bool is_white); //returns input mode of player in the argument
	void set_king_on_board(bool king_on_board, bool is_white); //changes if king of a colour is on board
	bool get_king_on_board(bool is_white);

	void set_piece_was_moved_this_turn(); //switches piece was moved to true
	bool get_piece_was_moved_this_turn(); 

	int get_moves_left();
	void set_moves_left(int num); 
	void set_to_delete_menu(bool val);
	std::pair<int, int> get_after_menu_coordinates();
	std::vector<Piece*> get_pieces();

	sf::Font font;
private:
	sf::RenderWindow window;
	sf::Text message_for_user;
	sf::Text who_is_on_move;

	std::vector<Button*> buttons; //vector of all buttons except menu buttons
	std::unique_ptr<Menu> activeMenu; //displayed menu when player has to choose from more options
	std::vector<Piece*> pieces; //vector of all pieces on board
	std::vector<Piece*> pieces_can_move; //pieces that can move this turn, used when there is restriction which pieces can move
	std::vector<Button*> last_clicked; //vector of last clicked buttons
	Gamestate gamestate = Gamestate::WHITE_TURN;
	InputMode white_input_mode = InputMode::NORMAL;
	InputMode black_input_mode = InputMode::NORMAL;
	MenuType menu_type = MenuType::NONE;

	std::pair<int, int> after_menu_dest_coordinates = { 0, 0 }; //pair to remember which square is attacked by a piece when menu is needed after attack
	
	bool to_delete_menu = false; //bool to remember if I want to delete a menu after the process of the menu is done
	bool white_king_on_board = true;
	bool black_king_on_board = true;
	bool piece_was_moved_this_turn = false; //literally if any piece moved from one square to another, used so rook can use its passive ability
	int moves_left = 1; //number of moves the player has until the gamestate changes and the opponent moves
};

#endif