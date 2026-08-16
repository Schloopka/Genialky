

#include "queen.h"
#include "../gamestate.h"
#include "../game.h"




Queen::Queen(bool is_white, int row, int column) {
	this->type = PieceType::QUEEN;
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->moves_when_attack = false;
	this->def_ability_reload = 6;
	this->attack_type = attackType::PHYSICAL;
	this->air_strike_data.phase = airStrikePhase::NOT_ACTIVE;
}


//returns true if it can move to the second square
MoveResult Queen::can_move_to(int dest_row, int dest_column, ON_TURN gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	if (std::abs(row - dest_row) <= 2 && std::abs(column - dest_column) <= 2
		&& (row != dest_row || column != dest_column)) {
		return MoveResult::VALID;
	}
	return MoveResult::NOT_VALID;

}

//returns true if it can interact with the second square as attack
MoveResult Queen::can_attack(int dest_row, int dest_column, ON_TURN gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//if the piece is reloading, it cant attack
	if (curr_reload > 0) {
		return MoveResult::REALOADING;
	}
	//if queen is after airstrike attack, it has to move
	bool is_after_air_strike_attack = game.get_input_mode(is_white) == InputMode::AIRSTRIKE_RESOLVE_ATTACK;
	if (std::abs(row - dest_row) <= 1 && std::abs(column - dest_column) <= 1
		&& (row != dest_row || column != dest_column) && game.get_king_on_board(is_white)
		&& !is_after_air_strike_attack) {
		instant_attack = false;
		return MoveResult::VALID;
	}
	else if (std::abs(row - dest_row) <= 2 && std::abs(column - dest_column) <= 2
		&& (row != dest_row || column != dest_column)) {
		instant_attack = true;
		moves_when_attack = true;
		return MoveResult::VALID;
	}
	return MoveResult::NOT_VALID;

}

std::vector<std::pair<int, int>> Queen::get_attacked_squares(int dest_row, int dest_column, Game& game) {

	return { {dest_row, dest_column} };
}

MoveResult Queen::can_activate_ability(ON_TURN gamestate, InputMode inputmode, Game& game) {

	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	if (curr_ability_reload > 0) {
		return MoveResult::REALOADING;
	}
	return MoveResult::VALID;
}

void Queen::activate_ability(ON_TURN gamestate, Game& game) {
	air_strike_data.original_column = this->column;
	air_strike_data.original_row = this->row;

	air_strike_data.phase = airStrikePhase::SELECTING_SQUARE;
	game.set_input_mode(gamestate, InputMode::AIRSTRIKE_SELECT_TARGET);

	this->moves_when_attack = true;

}

void Queen::ability_air_strike_select_square(int target_row, int target_column) {
	air_strike_data.target_row = target_row;
	air_strike_data.target_column = target_column;
	air_strike_data.phase = airStrikePhase::RESOLVING_ATTACK;
}

std::pair<int, int> Queen::get_air_strike_target_square() {
	return { air_strike_data.target_row, air_strike_data.target_column };
}
std::pair<int, int> Queen::get_air_strike_original_square() {
	return { air_strike_data.original_row, air_strike_data.original_column };
}
MoveResult Queen::can_be_eliminated(attackType attack_type, Game& game) {
	if (game.get_king_on_board(is_white)
		&& (attack_type == attackType::SHOOTING || attack_type == attackType::MAGIC)
		&& moves_since_last_took_a_piece <= 1) {
		return MoveResult::SHIELDED;
	}
	return MoveResult::VALID;
}
std::vector<MenuOption> Queen::get_menu_options(Game& game) {
	return { MenuOption::DONT_MOVE, MenuOption::MOVE_TO_ATTACKED_SQUARE };
}

