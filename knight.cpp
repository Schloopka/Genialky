#include <iostream>

#include "knight.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




Knight::Knight(bool is_white, int row, int column, int reload) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = reload;
	this->curr_reload = reload;
	this->moves_when_attack = true;
}

void Knight::display(sf::RenderWindow& window) {
	sf::Sprite knightSprite(knightTexture);
	knightSprite.setPosition({ 36.f + column * 100.f, 740.f - row * 100.f });
	knightSprite.setScale({ 1.0f, 0.9f });
	window.draw(knightSprite);

}
//returns true if it can move to the second square
bool Knight::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//mustang
	if ((curr_row == dest_row && std::abs(curr_column - dest_column) == 1) 
		|| (std::abs(curr_row - dest_row) == 1 && curr_column == dest_column)) {//can move one on column or row, not diagonally
		return true;
	}

	return false;

}

//returns true if it can interact with the second square as attack
bool Knight::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	if (curr_reload > 0) {
		return false;
	}
	
	if ((curr_row == dest_row && std::abs(curr_column - dest_column) == 2)
		|| (std::abs(curr_row - dest_row) == 2 && curr_column == dest_column)) {//can move one on column or row, not diagonally
		return true;
	}
	
	return false;

}
void Knight::attack(int dest_row, int dest_column, Game& game) {

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
	for (std::pair<int, int> square_coordinates : squares_to_stun) {
		for (auto& piece : game.get_pieces()) {
			if (piece->get_row() == square_coordinates.first && piece->get_column() == square_coordinates.second) {
				piece->set_curr_stun(1);
			}
		}
	}

	for (std::vector<int> square_coordinates : attacked_squares) {
		game.eliminate_pieces_from(square_coordinates.front(), square_coordinates.back());
	}
	
	for (auto& piece : game.get_pieces()) {
		if (piece->get_row() == get_row() && piece->get_column() == get_column()) {
			piece->move_piece_to(dest_row, dest_column);
		}
	}
	
}

bool Knight::can_be_eliminated() {
	return true;
}

std::vector<std::vector<int>> Knight::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {
	
	return { {dest_row, dest_column} };
}

bool Knight::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = knightTexture.loadFromFile("pieces/white-knight.png");
	}
	else {
		bool ok = knightTexture.loadFromFile("pieces/black-knight.png");
	}

	if (!ok) {
		std::cerr << "Failed to load textures." << std::endl;
		return false;
	}
	return true;
}