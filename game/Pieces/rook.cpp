#include <iostream>

#include "rook.h"

#include "../gamestate.h"
#include "../game.h"




Rook::Rook(bool is_white, int row, int column) {
	this->type = PieceType::ROOK;
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->moves_when_attack = true;
	this->poisons_when_attacks = false;
	this->attack_type = attackType::PHYSICAL;
	this->poison_attack = 1;
}


//returns true if it can move to the second square
MoveResult Rook::can_move_to(int dest_row, int dest_column, 
	Gamestate gamestate, InputMode inputmode, Game& game) {
	//We cant use the can_do_anything function, because it contains moves_left == 0 condition and rook can sometimes move when there are no moves left
	if (is_white == true && gamestate == Gamestate::BLACK_TURN) {
		return MoveResult::NOT_YOUR_TURN;
	}
	if (is_white == false && gamestate == Gamestate::WHITE_TURN) {
		return MoveResult::NOT_YOUR_TURN;
	}
	//check if the piece is in the pieces that can move this turn
	if (is_piece_in_can_move(game.get_pieces_can_move()) == false) {
		return MoveResult::CANT_MOVE_THIS_TURN;
	}
	if (curr_stun > 0) {
		return MoveResult::STUNNED;
	}
	/*if (inputmode == InputMode::AIRSTRIKE_SELECT_TARGET) {
		return MoveResult::SELECT_AIRSTRIKE_TARGET;
	}*/
	bool valid_coordinates = true;
	if ((row != dest_row || std::abs(column - dest_column) != 1) &&
		(column != dest_column || std::abs(row - dest_row) != 1)) {
		valid_coordinates = false;
	}
	//if king is on board and queen has just activated ability, rook can move, but cant move next move before airstrike is selected
	if (game.get_king_on_board(is_white) && game.get_moves_left() == 0 && game.get_input_mode(is_white) == InputMode::AIRSTRIKE_SELECT_TARGET
		&& valid_coordinates) {
		return MoveResult::VALID;
	}
	if (game.get_king_on_board(is_white) && game.get_piece_was_moved_this_turn() == false
		&& valid_coordinates) {
		return MoveResult::VALID;
	}

	return MoveResult::NOT_VALID;

}

//returns true if it can interact with the second square as attack
MoveResult Rook::can_attack(int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}	
	if ((row == dest_row && std::abs(column - dest_column) == 1) ||
		(column == dest_column && std::abs(row - dest_row) == 1)) {
		return MoveResult::VALID;
	}
	return MoveResult::NOT_VALID;

}

std::vector<std::pair<int, int>> Rook::get_attacked_squares(int dest_row, int dest_column, Game& game) {

	return { {dest_row, dest_column} };
}

MoveResult Rook::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	if (curr_ability_reload > 0) {
		return MoveResult::REALOADING;
	}

	return MoveResult::VALID;
}

void Rook::activate_ability(Gamestate gamestate, Game& game) {
	game.set_moves_left(4); //rook gets three moves but activating ability takes one
	this->poisons_when_attacks = true; //switches to poison mode
	curr_ability_reload = 1; //so the rook cannot call ability when its effect is active 
	set_poisoned_for(1); //rook dies after the ability is done
	this->curr_reload = 0;
	game.set_pieces_can_move({ this }); //only the rook can move now
}
MoveResult Rook::can_be_eliminated(attackType attack_type, Game& game) {
	if (game.get_king_on_board(is_white)) {
		set_poisoned_for(2);
		return MoveResult::NOT_VALID;
	}
	return MoveResult::VALID;
}


