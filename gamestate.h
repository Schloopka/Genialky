#pragma once

enum class Gamestate {
	WHITE_TURN,
	BLACK_TURN,
	END

};
enum class InputMode {
	NORMAL,
	AIRSTRIKE_SELECT_TARGET, //player is selecting target for airstrike
	AFTER_AIRSTRIKE_SELECT_TARGET, //after selecting target, player is again on the move
	AIRSTRIKE_RESOLVE_ATTACK, //resolving the airstrike attack and the queen can move again
	CHOOSING_FROM_MENU, //player is choosing from a menu of options	
	BISHOP_ABILITY
};
enum class airStrikePhase {
	NOT_ACTIVE,
	SELECTING_SQUARE,
	RESOLVING_ATTACK
};

enum class attackType {
	PHYSICAL,
	SHOOTING,
	MAGIC,
	ELIMINATION
};

enum class MenuType {
	NONE, 
	QUEEN_ATTACK
};

enum class MenuOption {
	//move when attack or not
	MOVE_TO_ATTACKED_SQUARE,
	DONT_MOVE,
	//promotion
	ROOK,
	BISHOP,
	QUEEN,
	KNIGHT
};

enum class PieceType {
	PAWN,
	BISHOP,
	KNIGHT,
	ROOK,
	QUEEN,
	KING
};

enum class MoveResult {
	VALID, 
	STUNNED,
	NOT_YOUR_TURN,
	CANT_MOVE_THIS_TURN,
	SELECT_AIRSTRIKE_TARGET,
	NO_MOVES_LEFT,
	REALOADING,
	CANT_ACTIVATE_ABILITY,
	SHIELDED,
	SHOT_OVER_PIECE,
	NOT_VALID
};

inline std::string move_result_to_string(MoveResult result) {
	switch (result) {
	case MoveResult::STUNNED: return "This piece is stunned";
	case MoveResult::NOT_YOUR_TURN: return "This piece is not on turn";
	case MoveResult::CANT_MOVE_THIS_TURN: return "This piece cannot move now";
	case MoveResult::SELECT_AIRSTRIKE_TARGET: return "Select airstrike target";
	case MoveResult::NO_MOVES_LEFT: return "No moves left this turn";
	case MoveResult::REALOADING: return "This piece is realoading";
	case MoveResult::CANT_ACTIVATE_ABILITY: return "You can't activate ability now";
	case MoveResult::SHIELDED: return "Target has shield";
	case MoveResult::SHOT_OVER_PIECE: return "Can't shoot over a piece";
	case MoveResult::NOT_VALID: return "You can't move this piece to that square";
	}
	return "Unknown state";
}

inline std::string menu_option_to_string(MenuOption option) {
	switch (option) {
	case MenuOption::MOVE_TO_ATTACKED_SQUARE:     return "Move piece";
	case MenuOption::DONT_MOVE:   return "Stay";
	case MenuOption::ROOK: return "Rook";
	case MenuOption::QUEEN: return "Queen";
	case MenuOption::KNIGHT: return "Knight";
	case MenuOption::BISHOP: return "Bishop";
	default:                   return "Unknown";
	}
}

