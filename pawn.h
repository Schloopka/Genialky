#pragma once

#include "piece.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

class Game;

class Pawn : public Piece {
public:
	Pawn(bool is_white, int row, int column, int reload);
	void display(sf::RenderWindow& window) override;
	bool can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	bool load_texture() override;
	bool can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game) override;
	bool can_be_eliminated();
	std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game) override; 

private:
	sf::Texture pawnTexture;
protected:
	std::string image;

};