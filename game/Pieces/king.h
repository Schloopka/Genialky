#pragma once

#include "piece.h"

class Game;

class King : public Piece {
public:
	King(bool is_white, int row, int column);
	MoveResult can_move_to(int dest_row, int dest_column, 
		ON_TURN gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_attack(int dest_row, int dest_column, 
		ON_TURN gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_be_eliminated(attackType attack_type, Game& game) override;
	std::vector<std::pair<int, int>> get_attacked_squares(int dest_row, int dest_column, Game& game) override;

	void activate_ability(ON_TURN gamestate, Game& game) override;
	MoveResult can_activate_ability(ON_TURN gamestate, InputMode inputmode, Game& game) override;

	
private:
	sf::Texture kingTexture;
	int ability_reload = 1;
protected:
	std::string image;
	

};
