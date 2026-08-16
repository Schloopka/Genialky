#pragma once

#include "piece.h"

class Game;

class Queen : public Piece {
public:
	Queen(bool is_white, int row, int column);
	MoveResult can_move_to(int dest_row, int dest_column, 
		ON_TURN gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_attack(int dest_row, int dest_column, 
		ON_TURN gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_be_eliminated(attackType attack_type, Game& game) override;
	std::vector<std::pair<int, int>> get_attacked_squares(int dest_row, int dest_column, Game& game) override;

	void activate_ability(ON_TURN gamestate, Game& game) override;
	MoveResult can_activate_ability(ON_TURN gamestate, InputMode inputmode, Game& game) override;

	void ability_air_strike_select_square(int target_row, int target_column) override;
	std::pair<int, int> get_air_strike_target_square() override;
	std::pair<int, int> get_air_strike_original_square() override; 
	std::vector<MenuOption> get_menu_options(Game& game) override;
private:
	sf::Texture queenTexture;
protected:
	std::string image;


};
