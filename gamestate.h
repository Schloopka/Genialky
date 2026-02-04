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
	CHOOSING_FROM_MENU //player is choosing from a menu of options	
};
enum class airStrikePhase {
	NOT_ACTIVE,
	SELECTING_SQUARE,
	RESOLVING_ATTACK
};

enum class attackType {
	PHYSICAL,
	SHOOTING,
	MAGIC
};

enum class MenuType {
	NONE, 
	QUEEN_ATTACK
};

enum class MenuOption {
	MOVE_TO_ATTACKED_SQUARE,
	DONT_MOVE
};

inline std::string to_string(MenuOption option) {
	switch (option) {
	case MenuOption::MOVE_TO_ATTACKED_SQUARE:     return "Move piece";
	case MenuOption::DONT_MOVE:   return "Stay";
	default:                   return "Unknown";
	}
}
