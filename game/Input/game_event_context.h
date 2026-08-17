#pragma once

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <string>
#include <vector>

class Button;
class Menu;

enum class GameActionType {
    NO_ACTION,
    SQUARE_MOVE,
    END_TURN_MOVE
};

struct GameEventContext {
    GameActionType action_type;
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
