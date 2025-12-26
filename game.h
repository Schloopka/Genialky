#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include "buttons.h"
#include "piece.h"
#include "gamestate.h"

class Game {
public:
	Game();
	void run();
	void setup();
	void render_background(sf::RenderWindow& window);
	void render_pieces(sf::RenderWindow& window); //renders pieces, called after every move
	void make_buttons(sf::RenderWindow& window); //make buttons at the start of the game, put them into vector
	void setup_pieces(sf::RenderWindow& window); //sets up pieces and puts them into a vector
	 
	void check_for_events(sf::RenderWindow& window, std::vector<Button*> buttons);//check for events and calls isClicked if button is clicked
	void handle_events(Button* button); //handles events when a button is clicked
	void handle_normal_moves(); //handles normal moves when two squares are clicked
	void handle_queen_select_airstrike(bool white_on_move); //handles events if the player must choose where queen lands after airstrike
	void clear_buttons_clicked(); //clears the vector of last clicked buttons after action with them is made

	void eliminate_pieces_from(int dest_row, int dest_column);//eliminate piece from a square, also checks, if the piece can be eliminated
	bool isThereAPiece(int row, int column); //returns true if there is a piece on that square
	bool only_one_piece_left(bool is_color_white);//returns true if there is only one piece left

	void lower_stats(); //after each turn, every piece of the person whose move ended has their stun, root or reload lowered by one (in this order)
	void switchGamestate(); //makes the opposite player move
	void switchGamestateAfterQueenAbility(bool white_on_move);


	void set_pieces_can_move(std::vector<Piece*> pieces);
	std::vector<Piece*> get_pieces_can_move();

	void set_input_mode(Gamestate gamestate, InputMode input_mode);//depending on gamestate, sets the input mode for that player

	void set_moves_left(int num); 
	std::vector<Piece*> get_pieces();


private:
	sf::RenderWindow window;
	sf::Font font;
	std::vector<Button*> buttons;
	std::vector<Piece*> pieces;
	std::vector<Piece*> pieces_can_move; //pieces that can move this turn, used when there is restriction which pieces can move
	std::vector<Button*> last_clicked; 
	Gamestate gamestate = Gamestate::WHITE_TURN;
	InputMode white_input_mode;
	InputMode black_input_mode;


	int moves_left = 1; //number of moves the player has until the gamestate changes and the opponent moves
};

#endif