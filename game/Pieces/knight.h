#pragma once

#include "piece.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


class Game;

class Knight : public Piece {
public:
	Knight(bool is_white, int row, int column);
	MoveResult can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	bool load_texture() override;
	MoveResult can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) override;

	void attack(int dest_row, int dest_column, Game& game, attackType attack_type) override;
private:
	sf::Texture knightTexture;
	int stun_lenght = 1;
protected:
	std::string image;

};
