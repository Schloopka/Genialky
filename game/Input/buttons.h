#pragma once

#include "menu.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

class Game;
enum class MenuOption;
class Button {
public:
	Button(sf::Vector2f position, sf::Vector2f size, int id, sf::Font& font, const std::string& text);
	Button(sf::Vector2f position, sf::Vector2f size, int id);

	Button() = default;
	virtual void isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game);
	virtual void draw_button(sf::RenderWindow& window);
	int get_id();
	int get_row();
	int get_column();
	const sf::RectangleShape& get_shape() const;
	const sf::Text& get_text() const;

protected: 
	sf::RectangleShape button_shape;
	sf::Text button_text;
	int id = 0;

};

class SquareButton : public Button {
private:
	int column ;
	int row;
	
public:
	SquareButton(sf::Vector2f position, sf::Vector2f size, int row, int column, int id, sf::Font& font);
	/*void isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game) override;*/

};

class MenuButton : public Button {
private:
	MenuOption menu_option;
public:
	MenuButton(sf::Vector2f position, sf::Vector2f size, int id, sf::Font& font, MenuOption menu_option);
	MenuButton(const MenuButton&) = delete;
	MenuButton& operator=(const MenuButton&) = delete;
	MenuOption get_menu_option();

};
