#include <iostream>

#include "bishop.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




Bishop::Bishop(bool is_white, int row, int column, int reload) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = reload;
	this->curr_reload = reload;
}

void Bishop::display(sf::RenderWindow& window) {
	sf::Sprite bishopSprite(bishopTexture);
	bishopSprite.setPosition({ 36.f + column * 100.f, 740.f - row * 100.f });
	bishopSprite.setScale({ 1.0f, 0.9f });
	window.draw(bishopSprite);

}
//returns true if it can move to the second square
bool Bishop::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//kušník
	if (std::abs(curr_row - dest_row) == 1 && std::abs(curr_column - dest_column) == 1) {//can move one diagonally
		return true;
	}

	return false;

}

//returns true if it can interact with the second square as attack
bool Bishop::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//if the piece is reloading, it cant attack
	if (curr_reload > 0) {
		return false;
	}
	//kušník
	if (std::abs(curr_row - dest_row) <= 3 && std::abs(curr_row - dest_row) != 0 &&
		std::abs(curr_column - dest_column) <= 3 && std::abs(curr_column - dest_column) != 0
		&& std::abs(curr_row - dest_row) == std::abs(curr_column - dest_column)) {//if the diagonal distance is less than 3 and it does not attack itsself
		return true;
	
	
	} 
	return false;

}
bool Bishop::can_be_eliminated() {
	return true;
}

std::vector<std::vector<int>> Bishop::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {
	//kušník
	if (dest_column > curr_column && dest_row > curr_row) {
		for (int i = 1; i < 4; i++) {
			if (game.isThereAPiece(curr_row + i, curr_column + i) == true) {
				return { {curr_row + i, curr_column + i} };
			}
		}
	}
	if (dest_column < curr_column && dest_row > curr_row) {
		for (int i = 1; i < 4; i++) {
			if (game.isThereAPiece(curr_row + i, curr_column - i) == true) {
				return { {curr_row + i, curr_column - i} };
			}
		}
	}
	if (dest_column > curr_column && dest_row < curr_row) {
		for (int i = 1; i < 4; i++) {
			if (game.isThereAPiece(curr_row - i, curr_column + i) == true) {
				return { {curr_row - i, curr_column + i} };
			}
		}
	}
	if (dest_column < curr_column && dest_row < curr_row) {
		for (int i = 1; i < 4; i++) {
			if (game.isThereAPiece(curr_row - i, curr_column - i) == true) {
				return { {curr_row - i, curr_column - i} };
			}
		}
	}
	return { {-1, -1} };
}

bool Bishop::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = bishopTexture.loadFromFile("pieces/white-bishop.png");
	}
	else {
		bool ok = bishopTexture.loadFromFile("pieces/black-bishop.png");
	}

	if (!ok) {
		std::cerr << "Failed to load textures." << std::endl;
		return false;
	}
	return true;
}