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
	int original_row = 0;
	int original_column = 0;

	int target_row = 0;
	int target_column = 0;

	airStrikePhase phase = airStrikePhase::NOT_ACTIVE;

};


class Piece {
public:
	Piece();
	virtual void display(sf::RenderWindow& window);
	virtual bool load_texture();

	void move_piece_to(int dest_row, int des_column);
	virtual bool can_move_to(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game);
	virtual bool can_be_eliminated(attackType attack_type, Game& game);
	virtual bool can_attack(int curr_row, int curr_column, int dest_row, int dest_column, Gamestate gamestate, InputMode inputmode, Game& game);
	virtual std::vector<std::vector<int>> get_attacked_squares(int curr_row, int curr_column, int dest_row, int dest_column, Game& game); //supposes that the piece can attack the square
	virtual void attack(int dest_row, int dest_column, Game& game, attackType attack_type);
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
	void set_ability_length(int num);
	int get_ability_length();
	int get_curr_stun(); //returns the current moves left  until the piece is not stunned
	void set_curr_stun(int num); //sets stun to num

	int get_poisoned_for() const;
	void set_poisoned_for(int num); //sets poison to num
	bool does_instant_attack();
	void set_instant_attack(bool atttack_instant);
	bool get_promotes();

	int get_moves_since_last_moved();
	void set_moves_since_last_moved(int num);

	int get_moves_since_last_took();
	void set_moves_since_last_took(int num);

	attackType get_attack_type();
	virtual PieceType get_type(); //this function is applied only to difference between queen and not queen
	//if you want to use it to other pieces, you have to update it in classes of concrete pieces

	airStrikePhase get_air_strike_phase();
	void set_air_strike_phase(airStrikePhase phase);
	virtual void ability_air_strike_select_square(int target_row, int target_column);
	virtual std::pair<int, int> get_air_strike_target_square();
	virtual std::pair<int, int> get_air_strike_original_square();

	virtual std::vector<MenuOption> get_menu_options(Game& game);

protected:
	PieceType type;
	int row = 0;
	int column = 0;
	bool is_white;
	bool has_moved = false;
	int moves_since_last_moved = 0;
	int moves_since_last_took_a_piece = 10; //initialized to "infinity", because no piece took another before the game

	bool moves_when_attack = false; //true if the piece moves to the square where it eliminated other pieces
	bool poisons_when_attacks = false; //true if the piece poisons the piece it attacks
	bool instant_attack = true; //true if the piece doesnt need menu when it attacks
	bool promotes = false; //if piece promotes when moved to last row
	
	int poison_attack = 0; //how many half-moves the piece poisons the piece it attacks for
	int def_reload = 0; //how many moves it takes the piece to reload
	int curr_reload = 0; //current state how many move left until the piece can attack again
	int def_ability_reload = 0; //how many moves the ability reloads
	int curr_ability_reload = 0; //how many moves are left until player can use the ability again
	int curr_ability_length = 0; //how many moves are left until the ability disappears
	int curr_stun = 0; //how many moves the piece is stunned

	int poisoned_for = 10000; //after how many half-moves the piece dies, this allows so some pieces to be eliminated just after they are poisoned
	//but also after some time, default set to "infinity"
	
	attackType attack_type;
	AirStrikeData air_strike_data{};

	std::string image;
};