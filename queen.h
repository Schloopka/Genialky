#pragma once

#include "piece.h"


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


class Game;

class Queen : public Piece {
public:
	Queen(bool is_white, int row, int column, int reload);
	void display(sf::RenderWindow& window) override;
	bool can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	bool load_texture() override;
	bool can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	bool can_be_eliminated() override;
	std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) override;

	void activate_ability(Gamestate gamestate, Game& game) override;
	bool can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game) override;

	void ability_air_strike_select_square(int target_row, int target_column) override;
	std::pair<int, int> get_air_strike_target_square() override;

private:
	sf::Texture queenTexture;
protected:
	std::string image;


};
