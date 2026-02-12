#include "piece.h"
#include "game.h"
#include <vector>

#include <iostream>
class Game;

Piece::Piece():row(0), column(0), is_white(true), attack_type(attackType::ELIMINATION){}

void Piece::display(sf::RenderWindow& window) {

}

bool Piece::load_texture() {
	return true;
}

int Piece::get_column() const{
	return this->column;
}

int Piece::get_row() const{
	return this->row;
}

bool Piece::is_piece_white() {
	if (is_white) {
		return true;
	}
	else {
		return false;
	}
}

bool Piece::get_moves_when_attack() {
	return this->moves_when_attack;
}

void Piece::set_moves_when_attack(bool val) {
	this->moves_when_attack = val;
}

void Piece::move_piece_to(int dest_row, int dest_column) {
	this->row = dest_row;
	this->column = dest_column;
	this->has_moved = true;
	this->moves_since_last_moved = -1; //set to -1, because at the end of the move, it is increased by 1
}

MoveResult Piece::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	return MoveResult::NOT_VALID;
}

MoveResult Piece::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	return MoveResult::NOT_VALID;
}

std::vector<std::vector<int>> Piece::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {
	std::vector<std::vector<int>> result = { {-1, -1} };
	return result;
}

void Piece::attack(int dest_row, int dest_column, Game& game, attackType attack_type) {
	std::vector<std::vector<int>> attacked_squares = this->get_attacked_squares(row, column, dest_row, dest_column, game);
	if (poisons_when_attacks == false) {
		for (std::vector<int> square_coordinates : attacked_squares) {
			game.eliminate_pieces_from(square_coordinates.front(), square_coordinates.back(), attack_type);
		}
	}
	else if (poisons_when_attacks == true) {
		for (std::vector<int> square_coordinates : attacked_squares) {
			for (auto& piece : game.get_pieces()) {
				if (piece->get_row() == square_coordinates.front() && piece->get_column() == square_coordinates.back()) {
					std::cout << piece->get_row() << piece->get_column()<< "poisoned for " << poison_attack << std::endl;
					piece->set_poisoned_for(poison_attack); 

				}
			}
		}
	}

	if (moves_when_attack) { 
		move_piece_to(dest_row, dest_column);
		game.set_piece_was_moved_this_turn();
	}
}

MoveResult Piece::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {
	return MoveResult::NOT_VALID;
}

MoveResult Piece::can_be_eliminated(attackType attack_type, Game& game) {
	return MoveResult::VALID;
}

MoveResult Piece::can_do_anything(Gamestate gamestate, InputMode inputmode, Game& game) {
	if (is_white == true && gamestate == Gamestate::BLACK_TURN) {
		return MoveResult::NOT_YOUR_TURN;
	}
	if (is_white == false && gamestate == Gamestate::WHITE_TURN) {
		std::cout << "not your turn" << std::endl;
		return MoveResult::NOT_YOUR_TURN;
	}
	//check if the piece is in the pieces that can move this turn
	if (is_piece_in_can_move(game.get_pieces_can_move()) == false) {
		std::cout << "cant move this turn" << std::endl;
		return MoveResult::CANT_MOVE_THIS_TURN;
	}
	if (curr_stun > 0) {
		std::cout << "stunned" << std::endl;
		return MoveResult::STUNNED;
	}
	if (inputmode == InputMode::AIRSTRIKE_SELECT_TARGET) {
		std::cout << "select airstrike target" << std::endl;
		return MoveResult::SELECT_AIRSTRIKE_TARGET;
	}
	if (game.get_moves_left() <= 0) {
		std::cout << "no moves left, you can move only piece which doesn't require a move" << std::endl;
		return MoveResult::NO_MOVES_LEFT;
	}
	return MoveResult::VALID;
}

void Piece::activate_ability(Gamestate gamestate, Game& game){
	
}

int Piece::get_reload() {
	return this->curr_reload;
}

void Piece::set_reload(int num) {
	this->curr_reload = num;
}

void Piece::reset_reload() {
	this->curr_reload = def_reload+1;//because the stats get lowered immediately after, so we need to add one more move for reloading
}

int Piece::get_ability_reload() {
	return this->curr_ability_reload;
}

void Piece::set_ability_reload(int num) {
	this->curr_ability_reload = num;
}
void Piece::set_ability_length(int num) {
	this->curr_ability_length = num;
}
int Piece::get_ability_length(){
	return curr_ability_length;
}
int Piece::get_curr_stun() {
	return this->curr_stun;
}

void Piece::set_curr_stun(int num) {
	this->curr_stun = num;
}

int Piece::get_poisoned_for() const {
	return this->poisoned_for;
}

void Piece::set_poisoned_for(int num) {
	this->poisoned_for = num;
}

bool Piece::does_instant_attack() {
	return this->instant_attack;
}

void Piece::set_instant_attack(bool attack_instant) {
	this->instant_attack = attack_instant;
}

bool Piece::get_promotes() {
	return this->promotes;
}

attackType Piece::get_attack_type() {
	return this->attack_type;
}

int Piece::get_moves_since_last_moved() {
	return this->moves_since_last_moved;
}
void Piece::set_moves_since_last_moved(int num) {
	this->moves_since_last_moved = num;
}

int Piece::get_moves_since_last_took() {
	return this->moves_since_last_took_a_piece;
}
void Piece::set_moves_since_last_took(int num) {
	this->moves_since_last_took_a_piece = num;
}
airStrikePhase Piece::get_air_strike_phase() {
	return this->air_strike_data.phase;
}

void Piece::set_air_strike_phase(airStrikePhase phase) {
	this->air_strike_data.phase = phase;
}

void Piece::ability_air_strike_select_square(int target_row, int target_column) {
	//default does nothing
}

std::pair<int, int> Piece::get_air_strike_target_square() {
	return { 0, 0 };
}
std::pair<int, int> Piece::get_air_strike_original_square() {
	return { 0, 0 };
}
PieceType Piece::get_type() {
	return PieceType::PAWN;
}
std::vector<MenuOption> Piece::get_menu_options(Game& game) {
	return {};
}

bool Piece::is_piece_in_can_move(std::vector<Piece*> pieces_can_move) {
	for (auto& piece : pieces_can_move) {
		if (piece == this) {
			return true;
		}
	}
	return false;
}