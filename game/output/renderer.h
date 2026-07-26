#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

class Game;
class Button;
class Menu;

class Renderer{
private:
    sf::RenderWindow& window;
    const sf::Color background = sf::Color(180, 180, 180);
public:
    Renderer(sf::RenderWindow& window);
    void render_background();
    void render_game(const Game& game);

    void render_buttons(const std::vector<Button*>& buttons);
    void draw_button(const Button& button);

    void render_menu(const Menu* menu);
};

#endif
