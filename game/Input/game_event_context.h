#pragma once

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <string>
#include <vector>

#include "../gamestate.h"

class Button;
class Menu;
class Piece;

enum class GameActionType {
    NO_ACTION,
    SQUARE_MOVE,
    END_TURN_MOVE,
    MENU_CLICK
};

struct GameEventContext {
    GameActionType action_type;
    MenuOption menu_option;
    bool is_input_player_white;
    bool is_singleplayer;
    std::vector<Button*>& last_clicked;
    std::string& action_description;
    std::unique_ptr<Menu>& menu;
    std::vector<Button*>& possible_actions;
    std::vector<Button*> buttons;
    sf::Vector2u window_size;

    Button* get_button(int row, int column) const;
};

// Server-owned state exposed to the client for drawing the current game.
// Keeping this separate from GameEventContext prevents the renderer from
// depending on the mutable Game object.
struct RenderContext {
    std::vector<Piece*> pieces;
    ON_TURN gamestate;
};
