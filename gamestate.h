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
	AIRSTRIKE_RESOLVE_ATTACK //resolving the airstrike attack and the queen can move again

};
enum class airStrikePhase {
	NOT_ACTIVE,
	SELECTING_SQUARE,
	RESOLVING_ATTACK
};