#pragma once

#include "piece.h"



class Game;

class Knight : public Piece {
public:
	Knight(bool is_white, int row, int column);
	MoveResult can_move_to(int dest_row, int dest_column, 
		ON_TURN gamestate, InputMode inputmode, Game& game) override;
	MoveResult can_attack(int dest_row, int dest_column, 
		ON_TURN gamestate, InputMode inputmode, Game& game) override;
	std::vector<std::pair<int, int>> get_attacked_squares(int dest_row, int dest_column, Game& game) override;

	void attack(int dest_row, int dest_column, Game& game, attackType attack_type) override;
private:
	sf::Texture knightTexture;
	int stun_lenght = 1;
protected:
	std::string image;

};
