#include <iostream>

#include "pawn.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"

Pawn::Pawn(bool is_white, int row, int column) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->attack_type = attackType::PHYSICAL;
	this->promotes = true;
}

void Pawn::display(sf::RenderWindow& window) {
	sf::Sprite pawnSprite(pawnTexture);
	pawnSprite.setPosition({ 38.f + column  * 100.f, 736.f - row  * 100.f });
	pawnSprite.setScale({ 1.0f, 0.9f });
	window.draw(pawnSprite);
	
}
//returns true if it can move to the second square
MoveResult Pawn::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//white moving forward - kopík
	if (is_white) {
		if (not has_moved) {
			if (curr_column == dest_column && (curr_row == dest_row - 2 || curr_row == dest_row - 1) 
				&& game.isThereAPiece(curr_row + 1, curr_column) == false /*check we dont go over any piece*/) {
				return MoveResult::VALID;
			}
		}
		if (has_moved) {
			if (curr_column == dest_column && curr_row == dest_row - 1) {
				return MoveResult::VALID;
			}
		}
	}
	if (not is_white) {
		if (not has_moved) {
			if (curr_column == dest_column && ( curr_row == dest_row + 2 || curr_row == dest_row + 1)
				&& game.isThereAPiece(curr_row - 1, curr_column) == false/*check we dont go over any piece*/) {
				return MoveResult::VALID;
			}
		}
		if (has_moved) {
			if (curr_column == dest_column && curr_row == dest_row + 1) {
				return MoveResult::VALID;
			}
		}
	}
	return MoveResult::NOT_VALID;

}

//returns true if it can interact with the second square as attack
MoveResult Pawn::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//if the piece is still reloading, it cant attack
	if (curr_reload > 0) {
		return MoveResult::REALOADING;
	}
	//check if white pawn can attack this square - kopík
	if (is_white) {
		if ((dest_row - curr_row == 1 && std::abs(dest_column - curr_column) == 1) || (dest_row - curr_row == 2 && std::abs(dest_column - curr_column) == 2)) {
			return MoveResult::VALID;
		}
	}
	if (not is_white) {
		if ((dest_row - curr_row == -1 && std::abs(dest_column - curr_column) == 1) || (dest_row - curr_row == -2 && std::abs(dest_column - curr_column) == 2)) {
			return MoveResult::VALID;
		}
	}
	return MoveResult::NOT_VALID;
}

MoveResult Pawn::can_be_eliminated(attackType attack_type, Game& game) {
	if (attack_type == attackType::SHOOTING && moves_since_last_moved >= 2) {
		std::cout << "I have shield" << std::endl;
		return MoveResult::SHIELDED;
	}
	return MoveResult::VALID;
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

std::vector<MenuOption> Pawn::get_menu_options(Game& game) {
	if (!game.get_king_on_board(is_white)) {
		return { MenuOption::BISHOP };
	}
	else {
		return { MenuOption::BISHOP, MenuOption::QUEEN, MenuOption::KNIGHT, MenuOption::ROOK };
	}
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