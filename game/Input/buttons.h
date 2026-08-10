#pragma once

#include "../gamestate.h"

#include <SFML/System/Vector2.hpp>

#include <string>
#include <functional>
class Game;

class Button {
public:
	using ClickHandler = std::function<void(Game&)>;

	Button(sf::Vector2f position, sf::Vector2f size, int id, std::string text = "",
		ClickHandler on_click = {});
	virtual ~Button() = default;

	int get_id() const;
	virtual int get_row() const;
	virtual int get_column() const;
	sf::Vector2f get_position() const;
	sf::Vector2f get_size() const;
	const std::string& get_text() const;
	bool contains(sf::Vector2f point) const;

	bool has_onlclick() const;
	void click(Game& game) const;

protected:
	sf::Vector2f position;
	sf::Vector2f size;
	std::string button_text;
	int id = 0;
	ClickHandler on_click;
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
