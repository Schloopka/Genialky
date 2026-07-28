#pragma once

#include "piece.h"


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


class Game;

class Queen : public Piece {
public:
	Queen(bool is_white, int row, int column);
	MoveResult can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_be_eliminated(attackType attack_type, Game& game) override;
	std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) override;

	void activate_ability(Gamestate gamestate, Game& game) override;
	MoveResult can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) override;

	void ability_air_strike_select_square(int target_row, int target_column) override;
	std::pair<int, int> get_air_strike_target_square() override;
	std::pair<int, int> get_air_strike_original_square() override; 
	std::vector<MenuOption> get_menu_options(Game& game) override;
private:
	sf::Texture queenTexture;
protected:
	std::string image;


};
