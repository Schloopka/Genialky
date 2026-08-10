#include "knight.h"

#include "../gamestate.h"
#include "../game.h"




Knight::Knight(bool is_white, int row, int column) {
	this->type = PieceType::KNIGHT;
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->moves_when_attack = true;
	this->attack_type = attackType::PHYSICAL;
}

//returns true if it can move to the second square
MoveResult Knight::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//mustang
	if ((curr_row == dest_row && std::abs(curr_column - dest_column) == 1) 
		|| (std::abs(curr_row - dest_row) == 1 && curr_column == dest_column)) {//can move one on column or row, not diagonally
		return MoveResult::VALID;
	}

	return MoveResult::NOT_VALID;

}

//returns true if it can interact with the second square as attack
MoveResult Knight::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	if (curr_reload > 0) {
		return MoveResult::REALOADING;
	}
	
	if ((curr_row == dest_row && std::abs(curr_column - dest_column) == 2)
		|| (std::abs(curr_row - dest_row) == 2 && curr_column == dest_column)) {//can move one on column or row, not diagonally
		stun_lenght = 1; //when knight moves two squares forward, then it stuns for one move
		return MoveResult::VALID;
	}
	if (((curr_row == dest_row && std::abs(curr_column - dest_column) == 3)
		|| (std::abs(curr_row - dest_row) == 3 && curr_column == dest_column))
		&& game.get_king_on_board(is_white)) {
		stun_lenght = 2; //when knight moves two squares forward, then it stuns for one move
		return MoveResult::VALID;

	}
	
	return MoveResult::NOT_VALID;

}
void Knight::attack(int dest_row, int dest_column, Game& game, attackType attack_type) {

	int id = 8 * this->get_row() + this->get_column(); /*remember id of the piece, so I can move it later, because when you remove a piece, the vector changes*/
	std::vector<std::vector<int>> attacked_squares = this->get_attacked_squares(this->get_row(), this->get_column(), dest_row, dest_column, game);

	//First find squares the knight stuns by the move
	std::vector<std::pair<int, int>> squares_to_stun = {};
	std::vector<std::pair<int, int>> directions = { {1,0}, {0,1}, {-1, 0}, {0, -1} };
	for (auto [row_move, column_move] : directions) {
		int stun_row = dest_row + row_move;
		int stun_column = dest_column + column_move;
		//coordinates of the square I dont want to stun
		int not_stun_row = (this->get_row() + dest_row) / 2; 
		int not_stun_column = (this->get_column() + dest_column) / 2;

		if (stun_row == not_stun_row && stun_column == not_stun_column) {
			continue;
			
		}
		else {
			squares_to_stun.push_back({ stun_row, stun_column });
		}
	}
	//
	for (std::pair<int, int> square_coordinates : squares_to_stun) {
		for (auto& piece : game.get_pieces()) {
			if (piece->get_row() == square_coordinates.first && piece->get_column() == square_coordinates.second) {
				piece->set_curr_stun(stun_lenght);
			}
		}
	}

	for (std::vector<int> square_coordinates : attacked_squares) {
		game.eliminate_pieces_from(square_coordinates.front(), square_coordinates.back(), attack_type);
	}
	
	for (auto& piece : game.get_pieces()) {
		if (piece->get_row() == get_row() && piece->get_column() == get_column()) {
			piece->move_piece_to(dest_row, dest_column);
		}
	}	
}

std::vector<std::vector<int>> Knight::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {
	
	return { {dest_row, dest_column} };
}

