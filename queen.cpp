#include <iostream>

#include "queen.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




Queen::Queen(bool is_white, int row, int column, int reload) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = reload;
	this->curr_reload = reload;
	this->moves_when_attack = false;
	this->def_ability_reload = 6;
	this->air_strike_data.phase = airStrikePhase::NOT_ACTIVE;
}

void Queen::display(sf::RenderWindow& window) {
	sf::Sprite queenSprite(queenTexture);
	//standard position
	if (air_strike_data.phase == airStrikePhase::NOT_ACTIVE) {
		queenSprite.setPosition({ 42.f + column * 100.f, 740.f - row * 100.f });
	}
	//ability is activated, show the queen on the special square
	else if ((air_strike_data.phase == airStrikePhase::SELECTING_SQUARE || air_strike_data.phase == airStrikePhase::RESOLVING_ATTACK)
		&& is_white){
		queenSprite.setPosition({ 893.f, 490.f });
	}
	else if (air_strike_data.phase == airStrikePhase::SELECTING_SQUARE || air_strike_data.phase == airStrikePhase::RESOLVING_ATTACK
		&& is_white == false) {
		queenSprite.setPosition({ 893.f, 290.f });
	}


	queenSprite.setScale({ 0.9f, 0.9f });
	window.draw(queenSprite);

}
//returns true if it can move to the second square
bool Queen::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	if (std::abs(curr_row - dest_row) <= 2 && std::abs(curr_column - dest_column) <= 2
		&& (curr_row != dest_row || curr_column != dest_column)) {
		return true;
	}
	return false;

}

//returns true if it can interact with the second square as attack
bool Queen::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	// check if the piece that should be moved is owned by the player that is on the move
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//if the piece is reloading, it cant attack
	if (curr_reload > 0) {
		return false;
	}
	if (std::abs(curr_row - dest_row) <= 1 && std::abs(curr_column - dest_column) <= 1
		&& (curr_row != dest_row || curr_column != dest_column)) {
		return true;
	}
	return false;

}

std::vector<std::vector<int>> Queen::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {

	return { {dest_row, dest_column} };
}

bool Queen::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {

	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	if (curr_ability_reload > 0) {
		return false;
	}
	return true;
}

void Queen::activate_ability(Gamestate gamestate, Game& game) {
	air_strike_data.original_column = this->column;
	air_strike_data.original_row = this->row;

	air_strike_data.phase = airStrikePhase::SELECTING_SQUARE;
	game.set_input_mode(gamestate, InputMode::AIRSTRIKE_SELECT_TARGET);
	game.switchGamestate();

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
bool Queen::can_be_eliminated() {
	return true;
}


bool Queen::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = queenTexture.loadFromFile("white-queen.png");
	}
	else {
		bool ok = queenTexture.loadFromFile("black-queen.png");
	}

	if (!ok) {
		std::cerr << "Failed to load textures." << std::endl;
		return false;
	}
	return true;
}