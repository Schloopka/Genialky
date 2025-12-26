#include <iostream>

#include "king.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




King::King(bool is_white, int row, int column, int reload) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = reload;
	this->curr_reload = reload;
	this->moves_when_attack = true;
	this->def_ability_reload = 1;
}

void King::display(sf::RenderWindow& window) {
	sf::Sprite kingSprite(kingTexture);
	kingSprite.setPosition({ 36.f + column * 100.f, 740.f - row * 100.f });
	kingSprite.setScale({ 1.0f, 0.9f });
	window.draw(kingSprite);

}
//returns true if it can move to the second square
bool King::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	
	if (std::abs(curr_row - dest_row) <= 1 && std::abs(curr_column - dest_column) <= 1 &&
		(std::abs(curr_row - dest_row) != 0 || std::abs(curr_column - dest_column) != 0)) {
		return true;
	}
	return false;

}

//returns true if it can interact with the second square as attack
bool King::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//if the piece is reloading, it cant attack
	if (curr_reload > 0) {
		return false;
	}
	if (std::abs(curr_row - dest_row) <= 1 && std::abs(curr_column - dest_column) <= 1 &&
		(std::abs(curr_row - dest_row) != 0 || std::abs(curr_column - dest_column) != 0)) {
		return true;
	}
	return false;

}

std::vector<std::vector<int>> King::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {

	return { {dest_row, dest_column} };
}

bool King::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	if (curr_ability_reload > 0) {
		return false;
	}
	if (is_white && game.only_one_piece_left(true)) {
		return true;
	}
	if (not is_white && game.only_one_piece_left(false)) {
		return true;
	}
	return false;
}

void King::activate_ability(Gamestate gamestate, Game& game){
	game.set_moves_left(3); 
	curr_ability_reload = def_ability_reload+1;

}
bool King::can_be_eliminated() {
	return true;
}


bool King::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = kingTexture.loadFromFile("white-king.png");
	}
	else {
		bool ok = kingTexture.loadFromFile("black-king.png");
	}

	if (!ok) {
		std::cerr << "Failed to load textures." << std::endl;
		return false;
	}
	return true;
}