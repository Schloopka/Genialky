#pragma once

#include "../gamestate.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

class Game;

class Button {
public:
	Button(sf::Vector2f position, sf::Vector2f size, int id, std::string text = "");
	virtual ~Button() = default;

	virtual void isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game);

	int get_id() const;
	virtual int get_row() const;
	virtual int get_column() const;
	sf::Vector2f get_position() const;
	sf::Vector2f get_size() const;
	const std::string& get_text() const;
	bool contains(sf::Vector2f point) const;

protected:
	sf::Vector2f position;
	sf::Vector2f size;
	std::string button_text;
	int id = 0;
};

class SquareButton : public Button {
private:
	int column;
	int row;

public:
	SquareButton(sf::Vector2f position, sf::Vector2f size, int row, int column, int id);

	int get_row() const override;
	int get_column() const override;
};

class MenuButton : public Button {
private:
	MenuOption menu_option;

public:
	MenuButton(sf::Vector2f position, sf::Vector2f size, int id, MenuOption menu_option);
	MenuButton(const MenuButton&) = delete;
	MenuButton& operator=(const MenuButton&) = delete;

	MenuOption get_menu_option() const;
};
