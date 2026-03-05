#include <iostream>

#include "bishop.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




Bishop::Bishop(bool is_white, int row, int column) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = 1;
	this->curr_reload = 1;;
	this->attack_type = attackType::SHOOTING;
}

void Bishop::display(sf::RenderWindow& window) {
	sf::Sprite bishopSprite(bishopTexture);
	bishopSprite.setPosition({ 36.f + column * 100.f, 740.f - row * 100.f });
	bishopSprite.setScale({ 1.0f, 0.9f });
	window.draw(bishopSprite);

}
//returns true if it can move to the second square
MoveResult Bishop::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//kušník
	if (std::abs(curr_row - dest_row) == 1 && std::abs(curr_column - dest_column) == 1) {//can move one diagonally
		return MoveResult::VALID;
	}

	return MoveResult::NOT_VALID;

}

//returns true if it can interact with the second square as attack
MoveResult Bishop::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//if the piece is reloading, it cant attack with shooting attack, but can do physical attack
	if (curr_reload == 0) {
		//kušník
		//if the diagonal distance is less than 3 and it does not attack itsself
		bool ability_activated = false;
		if ((is_white ? game.get_input_mode(true) : game.get_input_mode(false)) == InputMode::BISHOP_ABILITY) {
			ability_activated = true;
		}
		int max_shot_distance = (ability_activated ? 5 : 3);
		if ((std::abs(curr_row - dest_row) <= max_shot_distance && std::abs(curr_row - dest_row) != 0 &&
			std::abs(curr_column - dest_column) <= max_shot_distance && std::abs(curr_column - dest_column) != 0
			&& std::abs(curr_row - dest_row) == std::abs(curr_column - dest_column)))/*with king on board*/ {
			int distance = std::abs(curr_row - dest_row); //how far the bishop shoots, we need to check it doesnt shoot over any other piece
			bool shoot_up = (dest_row > curr_row ? true : false); //if shot is to higher row
			bool shoot_right = (dest_column > curr_column ? true : false);//if shot is to higher column (to the right)
			std::pair<int, int> directions = { (shoot_up ? 1 : -1), (shoot_right ? 1 : -1) };
			//check if there are no pieces in the way
			for (int i = 1; i < distance; i++) {

				if (game.isThereAPiece(curr_row + i * directions.first, curr_column + i * directions.second)) {
					return MoveResult::SHOT_OVER_PIECE;
				}
			}
			//next attack is shooting with reload 1
			attack_type = attackType::SHOOTING;
			def_reload = 1;
			return MoveResult::VALID;
		}
	}
	if (((std::abs(curr_row - dest_row) == 1 && curr_column == dest_column)
		|| (std::abs(curr_column - dest_column) == 1 && curr_row == dest_row))
		&& game.get_king_on_board(is_white)) {
		//next attack is physical with no reload
		attack_type = attackType::PHYSICAL;
		def_reload = 0;
		return MoveResult::VALID;
	}
	if (curr_reload > 0) {
		return MoveResult::REALOADING;
	}
	return MoveResult::NOT_VALID;

}

void Bishop::activate_ability(Gamestate gamestate, Game& game) {
	this->curr_ability_reload = 2;//one plus one because it is lowered by one after every move including this one
	this->curr_ability_length = 2;
	game.set_input_mode(gamestate, InputMode::BISHOP_ABILITY);
}

MoveResult Bishop::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	if (curr_ability_reload > 0) {
		return MoveResult::REALOADING;
	}
	return MoveResult::VALID;
}
MoveResult Bishop::can_be_eliminated(attackType attack_type, Game& game) {
	return MoveResult::VALID;
}

std::vector<std::vector<int>> Bishop::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {
	//kušník
	return { {dest_row, dest_column} };
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