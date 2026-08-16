#include <vector>
#include <algorithm>
#include <memory>

#include "game.h"
#include "Input/buttons.h"


class Buttons;
class PieceMenu;

Game::Game() {
	gamestate = ON_TURN::WHITE_TURN;
	white_input_mode = InputMode::NORMAL;
	black_input_mode = InputMode::NORMAL;
	setup();
}

Game::~Game() = default;

void Game::setup() {
	setup_pieces();
	set_all_pieces_can_move();
}

void Game::setup_pieces()
{
    for (int i = 0; i < 8; ++i) {
        pieces.emplace_back(std::make_unique<Pawn>(true, 1, i));
        pieces.emplace_back(std::make_unique<Pawn>(false, 6, i));
    }

    pieces.emplace_back(std::make_unique<Bishop>(true, 0, 2));
    pieces.emplace_back(std::make_unique<Bishop>(true, 0, 5));
    pieces.emplace_back(std::make_unique<Bishop>(false, 7, 2));
    pieces.emplace_back(std::make_unique<Bishop>(false, 7, 5));

    pieces.emplace_back(std::make_unique<Knight>(true, 0, 1));
    pieces.emplace_back(std::make_unique<Knight>(true, 0, 6));
    pieces.emplace_back(std::make_unique<Knight>(false, 7, 1));
    pieces.emplace_back(std::make_unique<Knight>(false, 7, 6));

    pieces.emplace_back(std::make_unique<King>(true, 0, 4));
    pieces.emplace_back(std::make_unique<King>(false, 7, 4));

    pieces.emplace_back(std::make_unique<Rook>(true, 0, 0));
    pieces.emplace_back(std::make_unique<Rook>(true, 0, 7));
    pieces.emplace_back(std::make_unique<Rook>(false, 7, 0));
    pieces.emplace_back(std::make_unique<Rook>(false, 7, 7));

    pieces.emplace_back(std::make_unique<Queen>(true, 0, 3));
    pieces.emplace_back(std::make_unique<Queen>(false, 7, 3));
}


void Game::handle_events(GameEventContext& context) {
	auto& last_clicked = context.last_clicked;
	InputMode on_move_input_mode = (gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode);
	context.possible_actions.clear();
	//more than two squares never interact, so we remove the first one and try again
	if (context.is_input_player_white != (gamestate == ON_TURN::WHITE_TURN)){
		context.last_clicked.clear();
	}
	//if square with no piece is clicked, it cannot ever activate a move
	else if (last_clicked.size() == 1 &&
		!is_there_a_piece(last_clicked[0]->get_row(), last_clicked[0]->get_column())
		&& on_move_input_mode != InputMode::AIRSTRIKE_SELECT_TARGET){
		last_clicked.clear();
	}
	//generate possible moves
	else if (last_clicked.size() == 1){
		create_possible_actions(context);
	}
	//handling bishop activity
	else if (last_clicked.size() == 2
		&& (on_move_input_mode) == InputMode::BISHOP_ABILITY) {
	for (auto& piece : pieces) {
		if (piece->get_ability_length() > 0 && piece->is_piece_white() == (gamestate == ON_TURN::WHITE_TURN)) {
			handle_bishop_abiltiy(gamestate == ON_TURN::WHITE_TURN, piece.get(), context);
			return;
		}
	}
	}
	//after the airstrike, queen gets one free move
	else if (last_clicked.size() == 2 &&
		on_move_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		handle_queen_after_landing(context);
		//on_move_input_mode = InputMode::NORMAL; //after the queen free move, then the white player can continue their turn normally
	}
	//queen ability - airstrike from the special square
	else if (last_clicked.size() == 2 && on_move_input_mode == InputMode::AIRSTRIKE_SELECT_TARGET &&
		last_clicked.front()->get_id() == (gamestate == ON_TURN::WHITE_TURN ? 65 : 66)
		&& moves_left != 0) {//white airstrike, first click is ability square
		handle_queen_select_airstrike(gamestate == ON_TURN::WHITE_TURN, context);
		moves_left = 1; //the player can make one more move after selecting target square
	}
	//normal move or ability activation
	else if (last_clicked.size() == 2 && last_clicked.back()->get_id() < 64) {
		handle_normal_moves(context);
	}
	//if nothing worked, we eliminate the first button clicked and try again
	//this allows that user can click on a piece, change his mind, click another one and move it 
	//and doesnt have to care about parity of number of clicks
	if (last_clicked.size() > 1 ) {
		last_clicked.erase(last_clicked.begin());
		handle_events(context);
	}
}

void Game::handle_queen_select_airstrike(bool white_on_move, GameEventContext& context) {
	auto& last_clicked = context.last_clicked;
	for (auto& piece : pieces) {
		if (piece->get_air_strike_phase() == airStrikePhase::SELECTING_SQUARE //piece is queen selecting square
			&& (white_on_move ? ON_TURN::WHITE_TURN : ON_TURN::BLACK_TURN) == gamestate //correct player is on move
			&& (piece->is_piece_white() == white_on_move) //the piece is of player who is on move
			&& std::abs(piece->get_air_strike_original_square().first - last_clicked.back()->get_row()) <= 4
			&& std::abs(piece->get_air_strike_original_square().second - last_clicked.back()->get_column()) <= 4)/*distance limitations*/ {
			piece->ability_air_strike_select_square(
				last_clicked.back()->get_row(),
				last_clicked.back()->get_column());
			(white_on_move ? white_input_mode : black_input_mode) = InputMode::AFTER_AIRSTRIKE_SELECT_TARGET;
			piece->set_air_strike_phase(airStrikePhase::RESOLVING_ATTACK);
			context.action_description = "Airstrike target selected";
			moves_left--;
		}
	}
	last_clicked.clear();
}

void Game::handle_queen_after_landing(GameEventContext& context) {
	auto& last_clicked = context.last_clicked;
	int dest_row = last_clicked.back()->get_row();
	int dest_column = last_clicked.back()->get_column();
	int original_row = last_clicked.front()->get_row();
	int original_column = last_clicked.front()->get_column();
	std::vector<Piece*> queens;
	std::vector<Piece*> pieces_without_queens;
	InputMode curr_inputmode = gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode;
	for (const auto& piece : pieces)
	{
		if (piece->get_type() != PieceType::QUEEN)
		{
			pieces_without_queens.emplace_back(piece.get());
		}
		else if (piece->is_piece_white() == (gamestate == ON_TURN::WHITE_TURN)){
			queens.emplace_back(piece.get()); //we select queens of only good colour
		}
	}
	bool queen_made_valid_move = false;

	for (auto& piece : queens) {
		//if the piece wasnt clicked, we dont want to move it
		if (original_row != piece->get_row() || original_column != piece->get_column()) {
			continue;
		}
		MoveResult move_result = piece->can_move_to(dest_row, dest_column, gamestate,
			curr_inputmode, *this);
		MoveResult attack_result = piece->can_attack(dest_row, dest_column, gamestate,
			curr_inputmode, *this);
		if (move_result == MoveResult::VALID //if the piece can move to the second square
			&& is_there_a_piece(dest_row, dest_column) == false) /*if there is no piece on the square it wants to go to*/ {
			piece->move_piece_to(dest_row, dest_column);
			context.action_description = "A piece was moved";
			piece_was_moved_this_turn = true;
			moves_left--;
			queen_made_valid_move = true;
		}
		else if (attack_result == MoveResult::VALID) { //if the piece can attack the second square
			if (piece->does_instant_attack()) {
				piece->attack(dest_row, dest_column, *this, piece->get_attack_type());
			}
			
			context.action_description = "A piece was attacked";
			moves_left--;
			queen_made_valid_move = true;
		}
		
	}
	/*If queen made a move, user gets one free move
	if they made another valid move, then after handle_normal_moves moves_left is equal to zero and user cant move
	if they made invalid move, moves left is at one, but this doesnt matter beacuse after they make a valid move with queen, it is still 1 and get get free move */
	if (queen_made_valid_move) {
		set_pieces_can_move(pieces_without_queens); //every piece can move after the queen free move
		moves_left = 1; //the player gets one move even if they made no move with the queen
		(gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode) = InputMode::NORMAL;
		last_clicked.clear();
	}
	else {
		moves_left = std::min(moves_left, 1);
		handle_normal_moves(context);
		
	}


}
void Game::handle_bishop_abiltiy(bool white_on_move, Piece* bishop, GameEventContext& context) {
	auto& last_clicked = context.last_clicked;
	int dest_row = last_clicked.back()->get_row();
	int dest_column = last_clicked.back()->get_column();
	MoveResult attack_result = bishop->can_attack(dest_row, dest_column, gamestate,
		gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode, *this);
	if (attack_result == MoveResult::VALID) {
		bishop->attack(dest_row, dest_column, *this, bishop->get_attack_type());
		context.action_description = "A piece was attacked";
		(gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode) = InputMode::NORMAL;
		last_clicked.clear();
	}
	else {
		handle_normal_moves(context);
	}
}

void Game::handle_normal_moves(GameEventContext& context) {
	auto& last_clicked = context.last_clicked;
	int dest_row = last_clicked.back()->get_row();
	int dest_column = last_clicked.back()->get_column();
	bool action_performed = false;
	Piece* piece_attacking = nullptr;
	InputMode curr_inputmode = gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode;
	for (auto& piece : pieces) {
		if (piece->is_piece_white() != (gamestate == ON_TURN::WHITE_TURN)
			|| piece->is_piece_white() == (gamestate == ON_TURN::BLACK_TURN)) {
			continue; //skips the piece if its not the turn of that color
		}
		else if (8 * piece->get_row() + piece->get_column() == (last_clicked.front()->get_id())) //if the piece is on the first clicked square
		{
			MoveResult move_result = piece->can_move_to(dest_row, dest_column, gamestate,
				curr_inputmode, *this);
			MoveResult attack_result = piece->can_attack(dest_row, dest_column, gamestate,
				curr_inputmode, *this);
			MoveResult ability_result = piece->can_activate_ability(gamestate,
				curr_inputmode, *this);
			//if piece can move to second clicked square, then it is moved there, also checks if there is no piece on that square
			if (move_result == MoveResult::VALID //if the piece can move to the second square
				&& is_there_a_piece(dest_row, dest_column) == false) /*if there is no piece on the square it wants to go to*/ {
				//if pawn promotes or not
				if (piece->get_promotes() && (dest_row == 0 || dest_row == 7)) {//we dont have to check the colour of the piece, pawn will never go to its own first row
					context.menu = std::make_unique<PieceMenu>(*piece, *this);
					context.menu->resize_buttons(context.window_size);
					after_menu_dest_coordinates = { dest_row, dest_column };
				}
				else {
					piece->move_piece_to(dest_row, dest_column);
				}
				context.action_description = "A piece was moved";
				piece_was_moved_this_turn = true;
				moves_left--;
				action_performed = true;
			}
			//if it cant move there, perhaps it can take a piece from that square
			else if (attack_result == MoveResult::VALID) { //if the piece can attack the second square
				piece_attacking = piece.get();
				if (piece->does_instant_attack()) {
					piece->attack(dest_row, dest_column, *this, piece->get_attack_type());
				}
				else {
					context.menu = std::make_unique<PieceMenu>(*piece, *this);
					context.menu->resize_buttons(context.window_size);
					after_menu_dest_coordinates = { dest_row, dest_column };
				}
				context.action_description = "A piece was attacked";
				moves_left--;
				action_performed = true;
			}
			else if (piece->get_row() == dest_row && piece->get_column() == dest_column
				&& ability_result==MoveResult::VALID) {
				piece->activate_ability(gamestate, *this);
				context.action_description = "Ability activated";
				moves_left--;
				action_performed = true;
			}
			else {
				MoveResult result_for_message = MoveResult::NOT_VALID;
				if (move_result != MoveResult::NOT_VALID && move_result != MoveResult::VALID) {
					result_for_message = move_result;
				}
				if (attack_result != MoveResult::NOT_VALID && attack_result != MoveResult::VALID) {
					result_for_message = attack_result;
				}
				if (ability_result != MoveResult::NOT_VALID && ability_result != MoveResult::VALID) {
					result_for_message = ability_result;
				}
				context.action_description = move_result_to_string(result_for_message);
			}
			// Attacking can erase an element from pieces, invalidating this
			// vector's iterators. The selected piece has now been handled, so
			// never advance the range-for iterator.
		}
		if (action_performed) {
			last_clicked.clear();
			break;
		}
	}
	
	if (piece_attacking != nullptr) {
		piece_attacking->reset_reload();
		piece_attacking->set_moves_since_last_took(0);
	}
}

void Game::try_end_turn(GameEventContext& context) {
	auto& last_clicked = context.last_clicked;
	if (context.is_input_player_white != (gamestate == ON_TURN::WHITE_TURN)){

	}
	else if (moves_left <= 0) {
		update_stats();
		switchGamestate(context);
		last_clicked.clear();
	}
	else {
		context.action_description = "You have to make a move until ending turn";
		last_clicked.clear(); //if player clicks to end move and cant end it,
		//we clear the buttons so it doesnt mess when they click on normal button again
	}
}

bool Game::eliminate_pieces_from(int dest_row, int dest_column, attackType attack_type) {
	int old_num_of_pieces = pieces.size();
	pieces.erase(
		std::remove_if(pieces.begin(), pieces.end(),
			[dest_row, dest_column, attack_type, this](const std::unique_ptr<Piece>& piece) {
				return piece->get_row() == dest_row && piece->get_column() == dest_column && 
				piece->can_be_eliminated(attack_type, *this)==MoveResult::VALID;
			}),
		pieces.end()
	);

	// pieces_can_move is a non-owning view. Remove pointers whose owners were
	// erased above before any later code can try to use them.
	pieces_can_move.erase(
		std::remove_if(
			pieces_can_move.begin(),
			pieces_can_move.end(),
			[this](const Piece* candidate) {
				return std::none_of(
					pieces.begin(),
					pieces.end(),
					[candidate](const std::unique_ptr<Piece>& piece) {
						return piece.get() == candidate;
					}
				);
			}
		),
		pieces_can_move.end()
	);

	int new_num_of_pieces = pieces.size();
	if (new_num_of_pieces == old_num_of_pieces) {
		return false;
	}
	else { 
		return true;
	}
}

bool Game::is_there_a_piece(int row, int column) {
	for (auto& piece : pieces) {
		if (piece->get_row() == row && piece->get_column() == column) {
			return true;
		}
	}
	return false;
}

void Game::switchGamestate(GameEventContext& context) {
	moves_left = 1; //resets the moves left
	piece_was_moved_this_turn = false;

	//eliminates poisoned pieces which poison ticked down to 0
	pieces.erase(std::remove_if(pieces.begin(), pieces.end(),
			[](const std::unique_ptr<Piece>& piece) {
				return piece->get_poisoned_for() == 0;
			}),
		pieces.end()
	);
	//any piece can move again
	set_all_pieces_can_move();
	InputMode& on_move_input_mode = (gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode);//who was on move before end of turn
	InputMode& opponent_input_mode = (gamestate == ON_TURN::WHITE_TURN ? black_input_mode : white_input_mode); //who will be on move after
	/*if the white player was selecting airstrike target, then he made normal move
	and he will start the next move by resolving the airstrike and moving the queen again*/
	if (on_move_input_mode == InputMode::AFTER_AIRSTRIKE_SELECT_TARGET) {
		on_move_input_mode = InputMode::AIRSTRIKE_RESOLVE_ATTACK;
	}
	else if (on_move_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		on_move_input_mode = InputMode::NORMAL;
	}
	//switches the gamestate
	gamestate = (gamestate == ON_TURN::WHITE_TURN ? ON_TURN::BLACK_TURN : ON_TURN::WHITE_TURN);
	//black is on the move, before the move, queen performs the airstrike attack and the player gets two moves
	//first must be with the queen (solved in handle_events)
	if (opponent_input_mode == InputMode::AIRSTRIKE_RESOLVE_ATTACK) {
		move_queen_back_to_board(gamestate == ON_TURN::WHITE_TURN);
	}
	if (opponent_input_mode == InputMode::BISHOP_ABILITY) {
		bool found_bishop_with_activated_ability = false;
		for (auto& piece : pieces) {
			if (piece->get_ability_length() > 0 && piece->is_piece_white() == (gamestate == ON_TURN::WHITE_TURN)) {
				found_bishop_with_activated_ability = true;
			}
		}
		if (!found_bishop_with_activated_ability) {
			opponent_input_mode = InputMode::NORMAL;
		}
	}
	context.action_description = gamestate == ON_TURN::WHITE_TURN ? "White on turn" : "Black on Turn";
}

void Game::move_queen_back_to_board(bool white_on_turn) {
	for (auto& piece : pieces) {
		if (piece->get_air_strike_phase() == airStrikePhase::RESOLVING_ATTACK && piece->is_piece_white() == white_on_turn) {//piece is the queen of player on turn
			//if white is on turn, we want to select white queen and vice versa
			int taget_row = piece->get_air_strike_target_square().first;
			int target_column = piece->get_air_strike_target_square().second;
			piece->set_air_strike_phase(airStrikePhase::NOT_ACTIVE); //the ability is over for the queen, so reset the state to default
			piece->set_ability_reload(7);
			piece->attack(taget_row, target_column, *this, piece->get_attack_type());
			break;
		}
	}
	moves_left = 2; //queen can move again after airstrike
}

bool Game::player_has_only_one_piece_left(bool is_player_white) {
	int count = 0;
	for (auto& piece : pieces) {
		if (piece->is_piece_white() == is_player_white) {
			count++;
		}
	}
	if (count > 1) {
		return false;
	}
	else {
		return true;
	}
}

std::vector<Piece*> Game::get_pieces_can_move() {
	return pieces_can_move;
}

void Game::set_pieces_can_move(std::vector<Piece*> pieces) {
	pieces_can_move = std::move(pieces);
}
void Game::set_all_pieces_can_move()
{
    pieces_can_move.clear();
    pieces_can_move.reserve(pieces.size());

    for (const auto& piece : pieces) {
        pieces_can_move.emplace_back(piece.get());
    }
}

void Game::set_input_mode(ON_TURN gamestate, InputMode input_mode) {
	if (gamestate == ON_TURN::WHITE_TURN) {
		white_input_mode = input_mode;
	}
	else if (gamestate == ON_TURN::BLACK_TURN) {
		black_input_mode = input_mode;
	}
}

InputMode Game::get_input_mode(bool is_white) {
	return (is_white ? white_input_mode : black_input_mode);
}

void Game::promote_piece(Piece& promoting_piece, std::pair<int, int> dest_coordinates, PieceType piece_type) {
	//add new piece
	switch (piece_type) {
	case PieceType::BISHOP:
		pieces.emplace_back(std::make_unique<Bishop>(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		break;
	case PieceType::KNIGHT:
		pieces.emplace_back(std::make_unique<Knight>(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		break;
	case PieceType::ROOK:
		pieces.emplace_back(std::make_unique<Rook>(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		break;
	case PieceType::QUEEN:
		pieces.emplace_back(std::make_unique<Queen>(promoting_piece.is_piece_white(), dest_coordinates.first, dest_coordinates.second));
		break;
	};
	//delete the pawn
	eliminate_pieces_from(promoting_piece.get_row(), promoting_piece.get_column(), attackType::ELIMINATION);
}

void Game::update_stats() {
	//after player's move, temporary stats of their pieces are lowered
	
	for (auto& piece : pieces) {
		if (piece->is_piece_white() == (gamestate == ON_TURN::WHITE_TURN )) {
			//if stun if lowered, then the piece doesnt reload
			if (piece->get_curr_stun() > 0) {
				piece->set_curr_stun(piece->get_curr_stun() - 1); //lowers stun by one
			}
			else if (piece->get_reload() > 0 || piece->get_ability_reload() > 0){
				piece->set_reload(std::max(0, piece->get_reload() - 1)); //lowers the reload by one
				piece->set_ability_reload(std::max(0, piece->get_ability_reload() - 1)); //lowers the ability reload by one
				piece->set_ability_length(std::max(0, piece->get_ability_length() - 1)); //ability length is lowered by one
			}
			piece->set_moves_since_last_moved(piece->get_moves_since_last_moved() + 1);
			piece->set_moves_since_last_took(piece->get_moves_since_last_took() + 1);
		}
	}
	
	//poison is in half-moves, because some pieces need to die right after their move/ability
	//and also poison opponent's piece to kill them after the move ends
	for (auto& piece : pieces) {
		piece->set_poisoned_for(std::max(0, piece->get_poisoned_for() - 1));
	}
}

std::vector<Piece*> Game::get_pieces() const {
	std::vector<Piece*> result;
    result.reserve(pieces.size());

    for (const auto& piece : pieces) {
        result.emplace_back(piece.get());
    }

    return result;
}

void Game::create_possible_actions(GameEventContext& context){
	const auto& last_clicked = context.last_clicked;
	int last_clicked_row = last_clicked[0]->get_row();
	int last_clicked_column = last_clicked[0]->get_column();
	InputMode input_mode = gamestate == ON_TURN::WHITE_TURN ? white_input_mode : black_input_mode;
	Piece* activated_piece = nullptr;
	for (auto piece : pieces_can_move){
		if (piece->get_row() == last_clicked_row && piece->get_column() == last_clicked_column){
			activated_piece = piece;
		}
	}
	if (activated_piece != nullptr){
		for (int r = 0; r < 8; r++){
			for (int c = 0; c < 8; c++){
				bool can_move = activated_piece->can_move_to(r, c, this->gamestate, input_mode, *this) == MoveResult::VALID;
				bool can_attack = activated_piece->can_attack(r, c, this->gamestate, input_mode, *this) == MoveResult::VALID;
				if (can_move || can_attack){
					context.possible_actions.emplace_back(context.get_button(r, c));
				}
			}
		}
		bool can_activate_ability = activated_piece->can_activate_ability(this->gamestate, input_mode, *this) == MoveResult::VALID;
		if (can_activate_ability){
			context.possible_actions.emplace_back(context.get_button(activated_piece->get_row(), activated_piece->get_column()));
		}
	}
}

ON_TURN Game::get_gamestate() const{
	return gamestate;
}

void Game::set_king_on_board(bool is_on_board, bool is_white) {
	if (is_white) {
		this->white_king_on_board = is_on_board;
	}
	else {
		this->black_king_on_board = is_on_board;
	}
}
bool Game::get_king_on_board(bool is_white) {
	return (is_white ? white_king_on_board : black_king_on_board);
}
void Game::set_piece_was_moved_this_turn() {
	this->piece_was_moved_this_turn = true;
}
bool Game::get_piece_was_moved_this_turn() {
	return this->piece_was_moved_this_turn;
}
void Game::set_moves_left(int num) {
	moves_left = num;
}
int Game::get_moves_left() {
	return this->moves_left;
}
std::pair<int, int> Game::get_after_menu_coordinates() {
	return this->after_menu_dest_coordinates;
}
