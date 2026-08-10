#pragma once

#include "piece.h"

class Game;

class Bishop : public Piece {
public:
	Bishop(bool is_white, int row, int column);
	MoveResult can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, 
		Gamestate gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_attack(int curr_row, int curr_column, int dest_row, int dest_column, 
		Gamestate gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_be_eliminated(attackType attack_type, Game& game) override;
	void activate_ability(Gamestate gamestate, Game& game) override;
	MoveResult can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) override;
	std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column,
		int dest_row, int dest_column, Game& game) override;

private:
	sf::Texture bishopTexture;
protected:
	std::string image;

};
