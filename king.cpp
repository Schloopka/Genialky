#include <iostream>

#include "king.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




King::King(bool is_white, int row, int column) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->moves_when_attack = true;
	this->def_ability_reload = 1;
	this->attack_type = attackType::PHYSICAL;
}

void King::display(sf::RenderWindow& window) {
	sf::Sprite kingSprite(kingTexture);
	kingSprite.setPosition({ 36.f + column * 100.f, 740.f - row * 100.f });
	kingSprite.setScale({ 1.0f, 0.9f });
	window.draw(kingSprite);

}
//returns true if it can move to the second square
MoveResult King::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	
	if (std::abs(curr_row - dest_row) <= 1 && std::abs(curr_column - dest_column) <= 1 &&
		(std::abs(curr_row - dest_row) != 0 || std::abs(curr_column - dest_column) != 0)) {
		return MoveResult::VALID;
	}
	return MoveResult::NOT_VALID;

}

//returns true if it can interact with the second square as attack
MoveResult King::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	//if the piece is reloading, it cant attack
	if (curr_reload > 0) {
		return MoveResult::REALOADING;
	}
	if (std::abs(curr_row - dest_row) <= 1 && std::abs(curr_column - dest_column) <= 1 &&
		(std::abs(curr_row - dest_row) != 0 || std::abs(curr_column - dest_column) != 0)) {
		return MoveResult::VALID;
	}
	return MoveResult::NOT_VALID;

}

std::vector<std::vector<int>> King::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {

	return { {dest_row, dest_column} };
}

MoveResult King::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) != MoveResult::VALID) {
		return can_do_anything(gamestate, inputmode, game);
	}
	if (curr_ability_reload > 0) {
		return MoveResult::REALOADING;
	}
	if (is_white && not game.player_has_only_one_piece_left(true)) {
		return MoveResult::CANT_ACTIVATE_ABILITY;
	}
	if (not is_white && not game.player_has_only_one_piece_left(false)) {
		return MoveResult::CANT_ACTIVATE_ABILITY;
	}
	return MoveResult::VALID;
}

void King::activate_ability(Gamestate gamestate, Game& game){
	game.set_moves_left(4); //king gets three moves but one is used to activate this ability
	curr_ability_reload = def_ability_reload+1;

}
MoveResult King::can_be_eliminated(attackType attack_type, Game& game) {
	game.set_king_on_board(false, is_white);
	return MoveResult::VALID;
}


bool King::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = kingTexture.loadFromFile("pieces/white-king.png");
	}
	else {
		bool ok = kingTexture.loadFromFile("pieces/black-king.png");
	}

	if (!ok) {
		std::cerr << "Failed to load textures." << std::endl;
		return false;
	}
	return true;
}