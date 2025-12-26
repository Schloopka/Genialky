#include <iostream>

#include "pawn.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"

Pawn::Pawn(bool is_white, int row, int column, int reload) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = reload;
	this->curr_reload = reload;
}

void Pawn::display(sf::RenderWindow& window) {
	sf::Sprite pawnSprite(pawnTexture);
	pawnSprite.setPosition({ 38.f + column  * 100.f, 736.f - row  * 100.f });
	pawnSprite.setScale({ 1.0f, 0.9f });
	window.draw(pawnSprite);
	
}
//returns true if it can move to the second square
bool Pawn::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//white moving forward - kopík
	if (is_white) {
		if (not has_moved) {
			if (curr_column != dest_column || curr_row != dest_row - 2 && curr_row != dest_row - 1 
				|| game.isThereAPiece(curr_row + 1, curr_column) /*check we dont go over any piece*/) {
				return false;
			}
		}
		if (has_moved) {
			if (curr_column != dest_column || curr_row != dest_row - 1) {
				return false;
			}
		}
	}
	if (not is_white) {
		if (not has_moved) {
			if (curr_column != dest_column || curr_row != dest_row + 2 && curr_row != dest_row + 1
				|| game.isThereAPiece(curr_row - 1, curr_column)/*check we dont go over any piece*/) {
				return false;
			}
		}
		if (has_moved) {
			if (curr_column != dest_column || curr_row != dest_row + 1) {
				return false;
			}
		}
	}
	return true;

}

//returns true if it can interact with the second square as attack
bool Pawn::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//if the piece is still reloading, it cant attack
	if (curr_reload > 0) {
		return false;
	}
	//check if white pawn can attack this square - kopík
	if (is_white) {
		if ((dest_row - curr_row == 1 && std::abs(dest_column - curr_column) == 1) || (dest_row - curr_row == 2 && std::abs(dest_column - curr_column) == 2)) {
			return true;
		}
	}
	if (not is_white) {
		if ((dest_row - curr_row == -1 && std::abs(dest_column - curr_column) == 1) || (dest_row - curr_row == -2 && std::abs(dest_column - curr_column) == 2)) {
			return true;
		}
	}	
}

bool Pawn::can_be_eliminated() {
	return true;
}

std::vector<std::vector<int>> Pawn::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {
	//white to the right
	if ((dest_row - curr_row == 1 && dest_column - curr_column == 1) || (dest_row - curr_row == 2 && dest_column - curr_column == 2)) {
		return { { curr_row + 1, curr_column + 1 },  {curr_row + 2, curr_column + 2 } };
	}
	//white to the left
	if ((dest_row - curr_row == 1 && dest_column - curr_column == -1) || (dest_row - curr_row == 2 && dest_column - curr_column == -2)) {
		return { { curr_row + 1, curr_column - 1 },  {curr_row + 2, curr_column - 2 } };
	}
	//black to the left
	if ((dest_row - curr_row == -1 && dest_column - curr_column == -1) || (dest_row - curr_row == -2 && dest_column - curr_column == -2)) {
		return { { curr_row - 1, curr_column - 1 },  {curr_row - 2, curr_column - 2 } };
	}
	//black to the right
	if ((dest_row - curr_row == -1 && dest_column - curr_column == 1) || (dest_row - curr_row == -2 && dest_column - curr_column == +2)) {
		return { { curr_row - 1, curr_column + 1 },  {curr_row - 2, curr_column + 2 } };
	}
	return { {-1, -1} };
}

bool Pawn::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = pawnTexture.loadFromFile("pieces/white-pawn.png");
	}
	else {
		bool ok = pawnTexture.loadFromFile("pieces/black-pawn.png");
	}
	
	if (!ok) {
		std::cerr << "Failed to load enemy textures." << std::endl;
		return false;
	}
	return true;
}