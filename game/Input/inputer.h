#pragma once
#ifndef INPUTER_H
#define INPUTER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

#include "buttons.h"
#include "menu.h"

enum class InputActionType {
    None,
    CloseWindow,
    ButtonClicked,
    MenuButtonClicked
};

struct InputAction {
    InputActionType type = InputActionType::None;
    Button* button = nullptr;
    MenuButton* menu_button = nullptr;

    InputAction(InputActionType type):type(type){}
    InputAction(InputActionType type, Button* button, MenuButton* menu_button)
        :type(type), button(button), menu_button(menu_button) {}
    InputAction(){}
};

class Inputer{
private:
    sf::RenderWindow& window;
    std::vector<Button*> last_clicked;
    MenuButton* find_clicked_menu_button(sf::Vector2f position, const Menu* menu, const sf::Event& event);
    Button* find_clicked_button(sf::Vector2f position, const std::vector<Button*>& buttons, const sf::Event& event);
public:
    Inputer(sf::RenderWindow& window);
    InputAction poll(const std::vector<Button*>& buttons, const Menu* active_menu);
};

#endif