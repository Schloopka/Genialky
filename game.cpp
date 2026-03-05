#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "game.h"

#include "piece.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "king.h"
#include "rook.h"
#include "queen.h"
#include "menu.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

class Buttons;
class PieceMenu;

Game::Game():window(sf::VideoMode({ 1200, 1000 }), "Genialky"), message_for_user(this->font), who_is_on_move(this->font)
{
	gamestate = Gamestate::WHITE_TURN;
	white_input_mode = InputMode::NORMAL;
	black_input_mode = InputMode::NORMAL;
	message_for_user.setString("Welcome to the game, white on turn");
	who_is_on_move.setString("White on move");
}

Game::~Game() = default;

void Game::setup() {
	if (!font.openFromFile("arial1.ttf")) {
		std::cout << "Error";
	}
	render_background(window);
	make_buttons(window);
	setup_pieces(window);
	set_pieces_can_move(pieces);
	setup_texts();
}

void Game::setup_pieces(sf::RenderWindow & window) {
	//pawns
	for (int i = 0; i < 8; i++) {
		//white
		pieces.push_back(new Pawn(true, 1, i));
	}

	for (int i = 0; i < 8; i++) {
		//black
		pieces.push_back(new Pawn(false, 6, i));
	}
	//bishops
	pieces.push_back(new Bishop(true, 0, 2));
	pieces.push_back(new Bishop(true, 0, 5));
	pieces.push_back(new Bishop(false, 7, 2));
	pieces.push_back(new Bishop(false, 7, 5));
	//knights
	pieces.push_back(new Knight(true, 0, 1));
	pieces.push_back(new Knight(true, 0, 6));
	pieces.push_back(new Knight(false, 7, 1));
	pieces.push_back(new Knight(false, 7, 6));
	//kings
	pieces.push_back(new King(true, 0, 4));
	pieces.push_back(new King(false, 7, 4));
	//rooks
	pieces.push_back(new Rook(true, 0, 0));
	pieces.push_back(new Rook(true, 0, 7));
	pieces.push_back(new Rook(false, 7, 0));
	pieces.push_back(new Rook(false, 7, 7));
	//queens
	pieces.push_back(new Queen(true, 0, 3));
	pieces.push_back(new Queen(false, 7, 3));
	//load textures
	for (auto& piece : pieces) {
		piece->load_texture();
	}
}
void Game::setup_texts() {
	message_for_user.setCharacterSize(30);
	message_for_user.setPosition({ 200.f,900.f });
	message_for_user.setFillColor(sf::Color::Black);
	who_is_on_move.setCharacterSize(30);
	who_is_on_move.setPosition({ 800.f,900.f });
	who_is_on_move.setFillColor(sf::Color::Black);
	
}

void Game::render_background(sf::RenderWindow& window) {
	sf::Color background(180, 180, 180);
	window.clear(background);
}

void Game::render_pieces(sf::RenderWindow& window) {
	for (auto& piece : pieces) {
		piece->display(window);
	}
}
void Game::render_buttons(sf::RenderWindow& window) {
	for (auto& button : buttons) {
		button->draw_button(window);
	}
	if (activeMenu != nullptr) {
		activeMenu->render(window);
	}

}

void Game::render_texts(sf::RenderWindow& window) {
	window.draw(message_for_user);
	window.draw(who_is_on_move);
}

void Game::set_message_for_user(std::string message) {
	this->message_for_user.setString(message);
}
void Game::make_buttons(sf::RenderWindow& window) {
	//board squares
	for (int c = 0; c < 8; c++) {
		for (int r = 0; r < 8; r++) {
			this->buttons.push_back(new SquareButton({ 50.f + 100.f * c, 750.f - 100.f * r }, {100.f, 100.f}, r, c, 8 * r + c, font));
		}
	}
	
	//queen ability squares
	this->buttons.push_back(new SquareButton({900.f, 500.f}, { 100.f, 100.f }, 8, 0, 65, font));
	this->buttons.push_back(new SquareButton({ 900.f, 300.f }, { 100.f, 100.f }, 8, 1, 66, font));

	//end turn button
	std::string text = "End turn";
	this->buttons.push_back(new Button({ 900.f, 425.f }, { 150.f, 50.f }, 67, font, text));
	
	
}

void Game::check_for_events(sf::RenderWindow& window, std::vector<Button*> buttons) {
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		if (event->is<sf::Event::MouseButtonPressed>()) {
			//if there is an active menu, we dont want to allow any other clicks
			if (activeMenu != nullptr) {
				activeMenu->process_clicks(window, *event, *this);
				//if menu is meant to be destroyed after something is clicked, 
				if (to_delete_menu) {
					activeMenu = nullptr;
					to_delete_menu = false;
				}
			}
			else {
				for (auto& squarebutton : buttons) {
					squarebutton->isClicked(window, *event, *this);
				}
				if (!last_clicked.empty()) {
					handle_events();
				}
			}
		}
	}
}

void Game::append_buttons_clicked(Button* button) {
	this->last_clicked.push_back(button);
}
void Game::handle_events() {
	std::string butt_row = std::to_string(last_clicked.back()->get_id() / 8 + 1);
	std::string butt_column = std::to_string(last_clicked.back()->get_id() % 8 + 1);
	set_message_for_user("Clicked square - row " + butt_row + " column " + butt_column);
	if (this->last_clicked.size() > 2) {
		this->last_clicked.clear();
	}
	else if (this->last_clicked.size() == 1) {
		handle_other_buttons();
	}
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::WHITE_TURN
		&& white_input_mode == InputMode::BISHOP_ABILITY) {
		for (auto& piece : pieces) {
			if (piece->get_ability_length() > 0 && piece->is_piece_white() == (gamestate == Gamestate::WHITE_TURN)) {
				handle_bishop_abiltiy(gamestate == Gamestate::WHITE_TURN, piece);
			}
		}
	}
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::BLACK_TURN
		&& black_input_mode == InputMode::BISHOP_ABILITY) {
		for (auto& piece : pieces) {
			if (piece->get_ability_length() > 0 && piece->is_piece_white() == (gamestate == Gamestate::WHITE_TURN)) {
				handle_bishop_abiltiy(gamestate == Gamestate::BLACK_TURN, piece);
			}
		}
	}
	//after the airstrike, queen gets one free move
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::WHITE_TURN &&
		white_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		handle_normal_moves(); //illegal move (which includes doubleclicking the same square) means the player doesnt want to move the queen after airstrike
		std::vector<Piece*> pieces_without_queens;
		for (const auto& piece : pieces)
		{
			if (piece->get_type() != PieceType::QUEEN)
			{
				pieces_without_queens.push_back(piece);
			}
		}
		set_pieces_can_move(pieces_without_queens); //every piece can move after the queen free move
		moves_left = 1; //the player gets one move even if they made no move with the queen
		white_input_mode = InputMode::NORMAL; //after the queen free move, then the white player can continue their turn normally
	}
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::BLACK_TURN &&
		black_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		handle_normal_moves(); //illegal move means the player doesnt want to move the queen after airstrike
		std::vector<Piece*> pieces_without_queens;
		for (const auto& piece : pieces)
		{
			if (piece->get_type() != PieceType::QUEEN)
			{
				pieces_without_queens.push_back(piece);
			}
		}
		set_pieces_can_move(pieces_without_queens); //every piece can move after the queen free move
		moves_left = 1; //the player gets one move even if they made no move with the queen
		black_input_mode = InputMode::NORMAL; //after the queen free move, then the white player can continue their turn normally
	}
	//queen ability - airstrike from the special square
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::WHITE_TURN &&
		white_input_mode == InputMode::AIRSTRIKE_SELECT_TARGET && this->last_clicked.front()->get_id() == 65
		&& moves_left != 0) {//white airstrike, first click is ability square
		handle_queen_select_airstrike(true);
		moves_left = 1; //the player can make one more move after selecting target square
	}
	else if (this->last_clicked.size() == 2 && gamestate == Gamestate::BLACK_TURN &&
		black_input_mode == InputMode::AIRSTRIKE_SELECT_TARGET && this->last_clicked.front()->get_id() == 66
		&& moves_left != 0) {//black airstrike, first click is ability square
		handle_queen_select_airstrike(false);
		moves_left = 1; //the player can make one more move after selecting target square
	}
	else if (this->last_clicked.size() == 2 && this->last_clicked.back()->get_id() < 64) {//normal move or ability activation
		handle_normal_moves();
	}
	else if (this->last_clicked.size() > 1 ) {
		this->last_clicked.erase(last_clicked.begin());
		handle_events();
	}
}

void Game::handle_queen_select_airstrike(bool white_on_move) {
	for (auto& piece : pieces) {
		if (piece->get_air_strike_phase() == airStrikePhase::SELECTING_SQUARE //piece is queen selecting square
			&& (white_on_move ? Gamestate::WHITE_TURN : Gamestate::BLACK_TURN) == gamestate //correct player is on move
			&& (piece->is_piece_white() == white_on_move) //the piece is of player who is on move
			&& std::abs(piece->get_air_strike_original_square().first - this->last_clicked.back()->get_id() / 8) <= 4
			&& std::abs(piece->get_air_strike_original_square().second - this->last_clicked.back()->get_id() % 8) <= 4)/*distance limitations*/ {
			piece->ability_air_strike_select_square(
				this->last_clicked.back()->get_id() / 8,
				this->last_clicked.back()->get_id() % 8);
			(white_on_move ? white_input_mode : black_input_mode) = InputMode::AFTER_AIRSTRIKE_SELECT_TARGET;
			piece->set_air_strike_phase(airStrikePhase::RESOLVING_ATTACK);
			set_message_for_user("Airstrike target selected");
			moves_left--;
		}
	}
	clear_buttons_clicked();
}

void Game::handle_bishop_abiltiy(bool white_on_move, Piece* bishop) {
	int dest_row = this->last_clicked.back()->get_id() / 8;
	int dest_column = this->last_clicked.back()->get_id() % 8;
	MoveResult attack_result = bishop->can_attack(bishop->get_row(), bishop->get_column(), dest_row, dest_column, gamestate,
		gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this);
	if (attack_result == MoveResult::VALID) {
		bishop->attack(dest_row, dest_column, *this, bishop->get_attack_type());
		set_message_for_user("A piece was attacked");
		(gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode) = InputMode::NORMAL;
		this->clear_buttons_clicked();
	}
	else {
		handle_normal_moves();
	}
}

void Game::handle_normal_moves() {
	int dest_row = this->last_clicked.back()->get_id() / 8;
	int dest_column = this->last_clicked.back()->get_id() % 8;
	bool clear_last_clicked = false;
	Piece* piece_attacking = nullptr;
	for (auto& piece : pieces) {
		if (piece->is_piece_white() != (gamestate == Gamestate::WHITE_TURN)
			|| piece->is_piece_white() == (gamestate == Gamestate::BLACK_TURN)) {
			continue; //skips the piece if its not the turn of that color
		}
		else if (8 * piece->get_row() + piece->get_column() == (this->last_clicked.front()->get_id())) //if the piece is on the first clicked square
		{
			MoveResult move_result = piece->can_move_to(piece->get_row(), piece->get_column(), dest_row, dest_column, gamestate,
				gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this);
			MoveResult attack_result = piece->can_attack(piece->get_row(), piece->get_column(), dest_row, dest_column, gamestate,
				gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this);
			MoveResult ability_result = piece->can_activate_ability(gamestate,
				gamestate == Gamestate::WHITE_TURN ? white_input_mode : black_input_mode, *this);
			//if piece can move to second clicked square, then it is moved there, also checks if there is no piece on that square
			if (move_result == MoveResult::VALID //if the piece can move to the second square
				&& isThereAPiece(dest_row, dest_column) == false) /*if there is no piece on the square it wants to go to*/ {
				//if pawn promotes or not
				if (piece->get_promotes() && (dest_row == 0 || dest_row == 7)) {//we dont have to check the colour of the piece, pawn will never go to its own first row
					activeMenu = std::make_unique<PieceMenu>(*piece, *this);
					after_menu_dest_coordinates = { dest_row, dest_column };
				}
				else {
					piece->move_piece_to(dest_row, dest_column);
				}
				set_message_for_user("A piece was moved");
				piece_was_moved_this_turn = true;
				moves_left--;
				clear_last_clicked = true;
			}
			//if it cant move there, perhaps it can take a piece from that square
			else if (attack_result == MoveResult::VALID) { //if the piece can attack the second square
				piece_attacking = piece;
				if (piece->does_instant_attack()) {
					piece->attack(dest_row, dest_column, *this, piece->get_attack_type());
				}
				else {
					activeMenu = std::make_unique<PieceMenu>(*piece, *this);
					after_menu_dest_coordinates = { dest_row, dest_column };
				}
				set_message_for_user("A piece was attacked");
				moves_left--;
				clear_last_clicked = true;
			}
			else if (piece->get_row() == dest_row && piece->get_column() == dest_column
				&& ability_result==MoveResult::VALID) {
				piece->activate_ability(gamestate, *this);
				set_message_for_user("Ability activated");
				moves_left--;
				clear_last_clicked = true;
			}
			else {
				MoveResult result_for_message = MoveResult::NOT_VALID;
				if (move_result != MoveResult::NOT_VALID && move_result != MoveResult::VALID) {
					result_for_message = move_result;
				}
				if (attack_result != MoveResult::NOT_VALID && attack_result != MoveResult::VALID) {
					result_for_message = attack_result;
				}
				if (ability_result != MoveResult::NOT_VALID && ability_result != MoveResult::VALID) {
					result_for_message = ability_result;
				}
				set_message_for_user(move_result_to_string(result_for_message));
			}
		}
	}
	if (clear_last_clicked) {
		this->clear_buttons_clicked();
	}
	if (last_clicked.size() > 1) {
		this->last_clicked.erase(last_clicked.begin());
		handle_events();
	}
	
	if (piece_attacking != nullptr) {
		piece_attacking->reset_reload();
		piece_attacking->set_moves_since_last_took(0);
	}
}

void Game::handle_other_buttons() {
	if (last_clicked.front()->get_id() == 67) {
		if (moves_left <= 0) {
			update_stats();
			switchGamestate();
			clear_buttons_clicked();
		}
		else {
			set_message_for_user("You have to make a move until ending turn");
			this->last_clicked.clear(); //if player clicks to end move and cant end it, 
			//we clear the buttons so it doesnt mess when they click on normal button again
		}

	}
}

void Game::clear_buttons_clicked() {
	this->last_clicked.clear();
}

void Game::eliminate_pieces_from(int dest_row, int dest_column, attackType attack_type) {
	pieces.erase(
		std::remove_if(pieces.begin(), pieces.end(),
			[dest_row, dest_column, attack_type, this](Piece* piece) {
				return piece->get_row() == dest_row && piece->get_column() == dest_column && piece->can_be_eliminated(attack_type, *this)==MoveResult::VALID;
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
	piece_was_moved_this_turn = false;

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
		if (black_input_mode == InputMode::BISHOP_ABILITY) {
			bool found_bishop_with_activated_ability = false;
			for (auto& piece : pieces) {
				if (piece->get_ability_length() > 0 && piece->is_piece_white() == (gamestate == Gamestate::WHITE_TURN)) {
					found_bishop_with_activated_ability = true;
				}
			}
			if (!found_bishop_with_activated_ability) {
				black_input_mode = InputMode::NORMAL;
			}
		}
		set_message_for_user("Black on turn");
		who_is_on_move.setString("Black on turn");
	}
	else if (gamestate == Gamestate::BLACK_TURN) {
		if (black_input_mode == InputMode::AFTER_AIRSTRIKE_SELECT_TARGET) {
			black_input_mode = InputMode::AIRSTRIKE_RESOLVE_ATTACK;
		}
		gamestate = Gamestate::WHITE_TURN;
		if (white_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
			switchGamestateAfterQueenAbility(true);
		}
		if (white_input_mode == InputMode::BISHOP_ABILITY) {
			bool found_bishop_with_activated_ability = false;
			for (auto& piece : pieces) {
				if (piece->get_ability_length() > 0 && piece->is_piece_white() == (gamestate == Gamestate::WHITE_TURN)) {
					found_bishop_with_activated_ability = true;
				}
			}
			if (!found_bishop_with_activated_ability) {
				white_input_mode = InputMode::NORMAL;
			}
		}
		set_message_for_user("White on turn");
		who_is_on_move.setString("White on turn");
	}
	
}

void Game::switchGamestateAfterQueenAbility(bool white_on_turn) {
	for (auto& piece : pieces) {
		if (piece->get_air_strike_phase() == airStrikePhase::RESOLVING_ATTACK && piece->is_piece_white() == white_on_turn) {//piece is the queen of player on turn
			//if white is on turn, we want to select white queen and vice versa
			int taget_row = piece->get_air_strike_target_square().first;
			int target_column = piece->get_air_strike_target_square().second;
			piece->set_air_strike_phase(airStrikePhase::NOT_ACTIVE); //the ability is over for the queen, so reset the state to default
			piece->set_ability_reload(7);
			set_pieces_can_move({ piece }); //after airstrike, only the queen can move
			piece->attack(taget_row, target_column, *this, piece->get_attack_type());
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

InputMode Game::get_input_mode(bool is_white) {
	return (is_white ? white_input_mode : black_input_mode);
}
void Game::promote_piece(Piece& promoting_piece, std::pair<int, int> dest_coordinates, PieceType piece_type) {
	//add new piece
	switch (piece_type) {
	case PieceType::BISHOP:
		pieces.push_back(new Bishop(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		pieces.back()->load_texture();
		break;
	case PieceType::KNIGHT:
		pieces.push_back(new Knight(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		pieces.back()->load_texture();
		break;
	case PieceType::ROOK:
		pieces.push_back(new Rook(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		pieces.back()->load_texture();
		break;
	case PieceType::QUEEN:
		pieces.push_back(new Queen(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		pieces.back()->load_texture();
		break;
	};
	//delete the pawn
	eliminate_pieces_from(promoting_piece.get_row(), promoting_piece.get_column(), attackType::ELIMINATION);
}

void Game::update_stats() {
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
					piece->set_ability_length(std::max(0, piece->get_ability_length() - 1)); //ability length is lowered by one
				}
				piece->set_moves_since_last_moved(piece->get_moves_since_last_moved() + 1);
				piece->set_moves_since_last_took(piece->get_moves_since_last_took() + 1);
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
					piece->set_ability_reload(std::max(0, piece->get_ability_reload() - 1));//lowers the ability reload by one
					piece->set_ability_length(std::max(0, piece->get_ability_length() - 1));//ability length is lowered by one
				}
				piece->set_moves_since_last_moved(piece->get_moves_since_last_moved() + 1);
				piece->set_moves_since_last_took(piece->get_moves_since_last_took() + 1);
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
void Game::set_king_on_board(bool is_on_board, bool is_white) {
	if (is_white) {
		this->white_king_on_board = is_on_board;
	}
	else {
		this->black_king_on_board = is_on_board;
	}
}
bool Game::get_king_on_board(bool is_white) {
	return (is_white ? white_king_on_board : black_king_on_board);
}
void Game::set_piece_was_moved_this_turn() {
	this->piece_was_moved_this_turn = true;
}
bool Game::get_piece_was_moved_this_turn() {
	return this->piece_was_moved_this_turn;
}
void Game::set_moves_left(int num) {
	moves_left = num;
}
int Game::get_moves_left() {
	return this->moves_left;
}
void Game::set_to_delete_menu(bool val) {
	this->to_delete_menu = val;
}

std::pair<int, int> Game::get_after_menu_coordinates() {
	return this->after_menu_dest_coordinates;
}
void Game::run() {
	while (window.isOpen()) {
		check_for_events(window, buttons);
		window.clear();
		this->render_background(window);
		this->render_buttons(window);
		this->render_pieces(window);
		this->render_texts(window);
		window.display();
	}
}