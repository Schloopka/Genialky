#include <iostream>
#include <vector>
#include <algorithm>
#include "game.h"

#include "piece.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "king.h"
#include "rook.h"
#include "queen.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

class Buttons;

Game::Game():window(sf::VideoMode({ 1200, 1000 }), "Genialky")
{
	gamestate = Gamestate::WHITE_TURN;
	white_input_mode = InputMode::NORMAL;
	black_input_mode = InputMode::NORMAL;
}

void Game::setup() {
	sf::Font font;
	if (!font.openFromFile("arial1.ttf")) {
		std::cout << "Error";
	}
	render_background(window);
	make_buttons(window);
	setup_pieces(window);
	set_pieces_can_move(pieces);
	

}

void Game::setup_pieces(sf::RenderWindow & window) {
	//pawns
	for (int i = 0; i < 8; i++) {
		//white
		pieces.push_back(new Pawn(true, 1, i, 3));
		pieces.back()->load_texture();
	}

	for (int i = 0; i < 8; i++) {
		//black
		pieces.push_back(new Pawn(false, 6, i, 3));
		pieces.back()->load_texture();
	}

	//bishops
	pieces.push_back(new Bishop(true, 0, 2, 1));
	pieces.back()->load_texture();
	pieces.push_back(new Bishop(true, 0, 5, 1));
	pieces.back()->load_texture();
	pieces.push_back(new Bishop(false, 7, 2, 1));
	pieces.back()->load_texture();
	pieces.push_back(new Bishop(false, 7, 5, 1));
	pieces.back()->load_texture();

	//knights
	pieces.push_back(new Knight(true, 0, 1, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Knight(true, 0, 6, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Knight(false, 7, 1, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Knight(false, 7, 6, 0));
	pieces.back()->load_texture();

	//kings
	pieces.push_back(new King(true, 0, 4, 0));
	pieces.back()->load_texture();
	pieces.push_back(new King(false, 7, 4, 0));
	pieces.back()->load_texture();

	//rooks
	pieces.push_back(new Rook(true, 0, 0, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Rook(true, 0, 7, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Rook(false, 7, 0, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Rook(false, 7, 7, 0));
	pieces.back()->load_texture();

	//queens
	pieces.push_back(new Queen(true, 0, 3, 0));
	pieces.back()->load_texture();
	pieces.push_back(new Queen(false, 7, 3, 0));
	pieces.back()->load_texture();

}

void Game::render_background(sf::RenderWindow& window) {
	sf::Color background(180, 180, 180);
	sf::Color white_square(248, 219, 161);
	sf::Color black_square(181, 136, 99);
	window.clear(background);
	sf::RectangleShape square({ 100.f, 100.f });

	//board
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			square.setPosition({ (750.f - 100.f * i), 750.f - 100.f * j });
			if ((i + j) % 2 == 0) {
				square.setFillColor(white_square);
			}
			else {
				square.setFillColor(black_square);
			}
			window.draw(square);
		}
	}

	//queen ability squares
	square.setPosition({ 900.f, 500.f });
	square.setFillColor(black_square);
	window.draw(square);

	square.setPosition({ 900.f, 300.f });
	square.setFillColor(white_square);
	window.draw(square);

}

void Game::render_pieces(sf::RenderWindow& window) {
	for (auto& piece : pieces) {
		piece->display(window);
	}
}

void Game::make_buttons(sf::RenderWindow& window) {
	//board squares
	for (int c = 0; c < 8; c++) {
		for (int r = 0; r < 8; r++) {
			this->buttons.push_back(new SquareButton({ 50.f + 100.f * c, 750.f - 100.f * r }, {100.f, 100.f}, r, c, 8 * r + c));
		}
	}
	
	//queen ability squares
	this->buttons.push_back(new Button({900.f, 500.f}, { 100.f, 100.f }, 65));
	this->buttons.push_back(new Button({ 900.f, 300.f }, { 100.f, 100.f }, 66));
	
}

void Game::check_for_events(sf::RenderWindow& window, std::vector<Button*> buttons) {
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		if (event->is<sf::Event::MouseButtonPressed>()) {
			for (auto& squarebutton : buttons) {
				squarebutton->isClicked(window, *event, *this);
			}
		}
	}
}

void Game::handle_events(Button* button) {
	this->last_clicked.push_back(button);
	std::cout << last_clicked.back()->get_id() << std::endl;
	//after the airstrike, queen gets one free move
	if (this->last_clicked.size() == 2 && gamestate == Gamestate::WHITE_TURN &&
		white_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		handle_normal_moves(); //illegal move (which includes doubleclicking the same square) means the player doesnt want to move the queen after airstrike
		set_pieces_can_move(pieces); //every piece can move after the queen free move
		moves_left = 1; //the player gets one move even if they made no move with the queen
		white_input_mode = InputMode::NORMAL; //after the queen free move, then the white player can continue their turn normally
	}
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::BLACK_TURN &&
		black_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		handle_normal_moves(); //illegal move means the player doesnt want to move the queen after airstrike
		set_pieces_can_move(pieces);
		moves_left = 1; //the player gets one move even if they made no move with the queen
		black_input_mode = InputMode::NORMAL; //after the queen free move, then the white player can continue their turn normally
	}
	//queen ability - airstrike from the special square
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::WHITE_TURN &&
		white_input_mode == InputMode::AIRSTRIKE_SELECT_TARGET && this->last_clicked.front()->get_id() == 65) {//white airstrike, first click is ability square
		handle_queen_select_airstrike(true);
	}
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::BLACK_TURN &&
		black_input_mode == InputMode::AIRSTRIKE_SELECT_TARGET && this->last_clicked.front()->get_id() == 66) {//black airstrike, first click is ability square
		handle_queen_select_airstrike(false);
	}
	else if (this->last_clicked.size() == 2 && this->last_clicked.back()->get_id() < 64) {//normal move or ability activation
		handle_normal_moves();
	}
}

void Game::handle_queen_select_airstrike(bool white_on_move) {
	for (auto& piece : pieces) {
		if (piece->get_air_strike_phase() == airStrikePhase::SELECTING_SQUARE
			&& (white_on_move ? Gamestate::WHITE_TURN : Gamestate::BLACK_TURN) == gamestate) {
			piece->ability_air_strike_select_square(
				this->last_clicked.back()->get_id() / 8,
				this->last_clicked.back()->get_id() % 8);
			(white_on_move ? white_input_mode : black_input_mode) = InputMode::AFTER_AIRSTRIKE_SELECT_TARGET;
			piece->set_air_strike_phase(airStrikePhase::RESOLVING_ATTACK);
			std::cout << "airstrike target selected" << std::endl;
		}
	}
	clear_buttons_clicked();
}

void Game::handle_normal_moves() {
	int dest_row = this->last_clicked.back()->get_id() / 8;
	int dest_column = this->last_clicked.back()->get_id() % 8;
	Piece* piece_attacking = nullptr;
	for (auto& piece : pieces) {
		if (piece->is_piece_white() != (gamestate == Gamestate::WHITE_TURN)
			|| piece->is_piece_white() == (gamestate == Gamestate::BLACK_TURN)) {
			continue; //skips the piece if its not the turn of that color
		}
		else if (8 * piece->get_row() + piece->get_column() == (this->last_clicked.front()->get_id())) //if the piece is on the first clicked square
		{
			//if piece can move to second clicked square, then it is moved there, also checks if there is no piece on that square
			if (piece->can_move_to(piece->get_row(), piece->get_column(), dest_row, dest_column, gamestate, 
				gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this)  //if the piece can move to the second square
				&& isThereAPiece(dest_row, dest_column) == false) /*if there is no piece on the square it wants to go to*/ {
				piece->move_piece_to(dest_row, dest_column);
				std::cout << "move" << std::endl;
				moves_left--;
				break;
			}
			//if it cant move there, perhaps it can take a piece from that square
			else if (piece->can_attack(piece->get_row(), piece->get_column(), dest_row, dest_column, gamestate, 
				gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this)) { //if the piece can attack the second square
				piece_attacking = piece;
				piece->attack(dest_row, dest_column, *this);
				std::cout << "take" << std::endl;
				moves_left--;
			}
			else if (piece->get_row() == dest_row && piece->get_column() == dest_column
				&& piece->can_activate_ability(gamestate, 
					gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this)) {
				piece->activate_ability(gamestate, *this);
				std::cout << "ability" << std::endl;
			}
		}
	}
	clear_buttons_clicked();
	if (piece_attacking != nullptr) {
		piece_attacking->reset_reload();
	}
	if (moves_left == 0) {
		lower_stats();
		switchGamestate();
	}
}

void Game::clear_buttons_clicked() {
	this->last_clicked.clear();
}

void Game::eliminate_pieces_from(int dest_row, int dest_column) {
	pieces.erase(
		std::remove_if(pieces.begin(), pieces.end(),
			[dest_row, dest_column](const Piece* piece) {
				return piece->get_row() == dest_row && piece->get_column() == dest_column;
			}),
		pieces.end()
	);
}

bool Game::isThereAPiece(int row, int column) {
	for (auto& piece : pieces) {
		if (piece->get_row() == row && piece->get_column() == column) {
			return true;
		}
	}
	return false;
}

void Game::switchGamestate() {
	moves_left = 1; //resets the moves left
	//eliminates poisoned pieces which poison ticked down to 0
	pieces.erase(std::remove_if(pieces.begin(), pieces.end(),
			[](const Piece* piece) {
				return piece->get_poisoned_for() == 0;
			}),
		pieces.end()
	);
	//any piece can move again
	set_pieces_can_move(pieces);
	//switch the gamestate and inputmode
	if (gamestate == Gamestate::WHITE_TURN) {
		/*if the white player was selecting airstrike target, then he made normal move
		and he will start the next move by resolving the airstrike and moving the queen again*/
		if (white_input_mode == InputMode::AFTER_AIRSTRIKE_SELECT_TARGET) {
			white_input_mode = InputMode::AIRSTRIKE_RESOLVE_ATTACK;
		}
		//switches the gamestate
		gamestate = Gamestate::BLACK_TURN;
		//black is on the move, before the move, queen performs the airstrike attack and the player gets two moves
		//first must be with the queen (solved in handle_events)
		if (black_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
			switchGamestateAfterQueenAbility(false);
		}
		std::cout << "black on turn" << std::endl;
	}
	else if (gamestate == Gamestate::BLACK_TURN) {
		if (black_input_mode == InputMode::AFTER_AIRSTRIKE_SELECT_TARGET) {
			black_input_mode = InputMode::AIRSTRIKE_RESOLVE_ATTACK;
		}
		gamestate = Gamestate::WHITE_TURN;
		if (white_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
			switchGamestateAfterQueenAbility(true);
		}
		std::cout << "white on turn" << std::endl;
	}
	
}

void Game::switchGamestateAfterQueenAbility(bool white_on_turn) {
	for (auto& piece : pieces) {
		if (piece->get_air_strike_phase() == airStrikePhase::RESOLVING_ATTACK && piece->is_piece_white() == white_on_turn) {//piece is the queen of player on turn
			//if white is on turn, we want to select white queen and vice versa
			int taget_row = piece->get_air_strike_target_square().first;
			int target_column = piece->get_air_strike_target_square().second;
			piece->set_air_strike_phase(airStrikePhase::NOT_ACTIVE); //the ability is over for the queen, so reset the state to default
			piece->set_ability_reload(6);
			set_pieces_can_move({ piece }); //after airstrike, only the queen can move
			piece->attack(taget_row, target_column, *this);
			piece->set_moves_when_attack(false); //
		}
	}
	moves_left = 2; //black queen can move again after airstrike
}

bool Game::only_one_piece_left(bool is_color_white) {
	int count = 0;
	if (is_color_white == true) {
		for (auto& piece : pieces) {
			if (piece->is_piece_white() == true) {
				count++;
			}
		}
	}
	if (is_color_white == false) {
		for (auto& piece : pieces) {
			if (piece->is_piece_white() == false) {
				count++;
			}
		}
	}
	if (count > 1) {
		return false;
	}
	else {
		return true;
	}
}

std::vector<Piece*> Game::get_pieces_can_move() {
	return pieces_can_move;
}

void Game::set_pieces_can_move(std::vector<Piece*> pieces) {
	this->pieces_can_move = pieces;
}

void Game::set_input_mode(Gamestate gamestate, InputMode input_mode) {
	if (gamestate == Gamestate::WHITE_TURN) {
		white_input_mode = input_mode;
	}
	else if (gamestate == Gamestate::BLACK_TURN) {
		black_input_mode = input_mode;
	}
}

void Game::lower_stats() {
	//after player's move, temporary stats of their pieces are lowered
	if (gamestate == Gamestate::WHITE_TURN) {
		for (auto& piece : pieces) {
			if (piece->is_piece_white()) {			
				//if stun if lowered, then the piece doesnt reload
				if (piece->get_curr_stun() > 0) {
					piece->set_curr_stun(piece->get_curr_stun() - 1); //lowers stun by one
				}
				else if (piece->get_reload() > 0 || piece->get_ability_reload() > 0){
					piece->set_reload(std::max(0, piece->get_reload() - 1)); //lowers the reload by one
					piece->set_ability_reload(std::max(0, piece->get_ability_reload() - 1)); //lowers the ability reload by one
				}
			}
		}
	}
	if (gamestate == Gamestate::BLACK_TURN) {
		for (auto& piece : pieces) {
			if (piece->is_piece_white() == false) {
				if (piece->get_curr_stun() > 0) {
					piece->set_curr_stun(piece->get_curr_stun() - 1); //lowers stun by one
				}
				else if (piece->get_reload() > 0 || piece->get_ability_reload() > 0) {
					piece->set_reload(std::max(0, piece->get_reload() - 1)); //lowers the reload by one
					piece->set_ability_reload(std::max(0, piece->get_ability_reload() - 1));
				}
			}
		}
	}
	//poison is in half-moves, because some pieces need to die right after their move/ability
	//and also poison opponent's piece to kill them after the move ends
	for (auto& piece : pieces) {
		piece->set_poisoned_for(std::max(0, piece->get_poisoned_for() - 1));
	}
}

std::vector<Piece*> Game::get_pieces() {
	return pieces;
}

void Game::set_moves_left(int num) {
	moves_left = num;
}

void Game::run() {
	while (window.isOpen()) {
		check_for_events(window, buttons);
		window.clear();
		this->render_background(window);
		this->render_pieces(window);
		
		window.display();
	}
}