#pragma once

#include "piece.h"

class Game;

class Pawn : public Piece {
public:
	Pawn(bool is_white, int row, int column);
	MoveResult can_move_to(int dest_row, int dest_column, 
		Gamestate gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_attack(int dest_row, int dest_column, 
		Gamestate gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_be_eliminated(attackType attack_type, Game& game) override;
	std::vector<std::pair<int, int>> get_attacked_squares(int dest_row, int dest_column, Game& game) override; 
	std::vector<MenuOption> get_menu_options(Game& game) override;
	void attack(int dest_row, int dest_column, Game& game, attackType attack_type) override;
private:
	sf::Texture pawnTexture;
protected:
	std::string image;

};