#include <iostream>

#include "rook.h"
#include "piece.h"
#include "gamestate.h"
#include "game.h"




Rook::Rook(bool is_white, int row, int column, int reload) {
	this->is_white = is_white;
	this->row = row;
	this->column = column;
	this->def_reload = reload;
	this->curr_reload = reload;
	this->moves_when_attack = true;
	this->poisons_when_attacks = false;
	this->attack_type = attackType::PHYSICAL;
	this->poison_attack = 1;
}

void Rook::display(sf::RenderWindow& window) {
	sf::Sprite rookSprite(rookTexture);
	rookSprite.setPosition({ 36.f + column * 100.f, 740.f - row * 100.f });
	rookSprite.setScale({ 1.0f, 0.9f });
	window.draw(rookSprite);

}
//returns true if it can move to the second square
bool Rook::can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	//We cant use the can_do_anything function, because it contains moves_left == 0 condition and rook can sometimes move when there are no moves left
	if (is_white == true && gamestate == Gamestate::BLACK_TURN) {
		std::cout << "not your turn" << std::endl;
		return false;
	}
	if (is_white == false && gamestate == Gamestate::WHITE_TURN) {
		std::cout << "not your turn" << std::endl;
		return false;
	}
	//check if the piece is in the pieces that can move this turn
	if (is_piece_in_can_move(game.get_pieces_can_move()) == false) {
		std::cout << "cant move this turn" << std::endl;
		return false;
	}
	if (curr_stun > 0) {
		std::cout << "stunned" << std::endl;
		return false;
	}
	if (inputmode == InputMode::AIRSTRIKE_SELECT_TARGET) {
		std::cout << "select airstrike target" << std::endl;
		return false;
	}
	if ((curr_row == dest_row && std::abs(curr_column - dest_column) == 1) ||
		(curr_column == dest_column && std::abs(curr_row - dest_row) == 1) //valid position change
		&& (game.get_moves_left() > 0 || //either there are moves left
			(game.get_king_on_board(is_white)&& game.get_piece_was_moved_this_turn()==false))/*or king is on board and rook can use its passive*/) {
		return true;
	}

	return false;

}

//returns true if it can interact with the second square as attack
bool Rook::can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	//if the piece is reloading, it cant attack
	if (curr_reload > 0) {
		return false;
	}
	
	if ((curr_row == dest_row && std::abs(curr_column - dest_column) == 1) ||
		(curr_column == dest_column && std::abs(curr_row - dest_row) == 1)) {
		return true;
	}
	return false;

}

std::vector<std::vector<int>> Rook::get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) {

	return { {dest_row, dest_column} };
}

bool Rook::can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) {
	if (can_do_anything(gamestate, inputmode, game) == false) {
		return false;
	}
	if (curr_ability_reload > 0) {
		return false;
	}

	return true;
}

void Rook::activate_ability(Gamestate gamestate, Game& game) {
	game.set_moves_left(3); 
	this->poisons_when_attacks = true; //switches to poison mode
	curr_ability_reload = 1; //so the rook cannot call ability when its effect is active 
	set_poisoned_for(1); //rook dies after the ability is done
	this->curr_reload = 0;
	game.set_pieces_can_move({ this }); //only the rook can move now
}
bool Rook::can_be_eliminated(attackType attack_type, Game& game) {
	if (game.get_king_on_board(is_white)) {
		set_poisoned_for(2);
		return false;
	}
	return true;
}


bool Rook::load_texture() {

	bool ok = true;
	if (is_white) {
		bool ok = rookTexture.loadFromFile("pieces/white-rook.png");
	}
	else {
		bool ok = rookTexture.loadFromFile("pieces/black-rook.png");
	}

	if (!ok) {
		std::cerr << "Failed to load textures." << std::endl;
		return false;
	}
	return true;
}