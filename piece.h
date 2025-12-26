#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include "gamestate.h"
#include "buttons.h"

class Game;


struct AirStrikeData
{
	int original_row;
	int original_column;

	int target_row;
	int target_column;

	airStrikePhase phase = airStrikePhase::NOT_ACTIVE;

};

class Piece {
public:
	Piece();
	virtual void display(sf::RenderWindow& window);
	virtual bool load_texture();

	void move_piece_to(int dest_row, int des_column);
	virtual bool can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game);
	virtual bool can_be_eliminated();
	virtual bool can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game);
	virtual std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game); //supposes that the piece can attack the square
	virtual void attack(int dest_row, int dest_column, Game& game);
	virtual void activate_ability(Gamestate gamestate, Game& game);
	virtual bool can_activate_ability(Gamestate gamestate, InputMode inputmode, Game& game);//returns true if the piece can activate special activity, which is not a move or a take

	bool can_do_anything(Gamestate gamestate, InputMode inputmode, Game& game); //check if the piece is not stunned, player is on move, piece is in pieces that can move
	bool is_piece_in_can_move(std::vector<Piece*> pieces_can_move);
	bool is_piece_white(); //returns true if the piece is white

	bool get_moves_when_attack();
	void set_moves_when_attack(bool val);

	int get_row() const;
	int get_column() const;

	int get_reload();
	void set_reload(int num); //used ie when you need to lower current reload
	void reset_reload(); //used when the piece attacks and sets the current reload to default reload

	int get_ability_reload();
	void set_ability_reload(int num);

	int get_curr_stun(); //returns the current moves left  until the piece is not stunned
	void set_curr_stun(int num); //sets stun to num

	int get_poisoned_for() const;
	void set_poisoned_for(int num); //sets poison to num

	airStrikePhase get_air_strike_phase();
	void set_air_strike_phase(airStrikePhase phase);
	virtual void ability_air_strike_select_square(int target_row, int target_column);
	virtual std::pair<int, int> get_air_strike_target_square();

protected:
	int row;
	int column;
	bool is_white;
	bool has_moved = false;

	bool moves_when_attack = false; //true if the piece moves to the square where it eliminated other pieces
	bool poisons_when_attacks = false; //true if the piece poisons the piece it attacks

	int poison_attack = 0; //how many half-moves the piece poisons the piece it attacks for
	int def_reload = 0; //how many moves it takes the piece to reload
	int curr_reload = 0; //current state how many move left until the piece can attack again
	int def_ability_reload = 0;
	int curr_ability_reload = 0;
	int curr_stun = 0; //how many moves the piece is stunned

	int poisoned_for = 10000; //after how many half-moves the piece dies, this allows so some pieces to be eliminated just after they are poisoned
	//but also after some time, default set to "infinity"

	AirStrikeData air_strike_data;

	std::string image;
};