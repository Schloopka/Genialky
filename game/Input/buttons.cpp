#include "buttons.h"
#include "../game.h"

#include <utility>

Button::Button(
	sf::Vector2f position,
	sf::Vector2f size,
	int id,
	std::string text
) : position(position), size(size), button_text(std::move(text)), id(id) {}

void Button::isClicked(
	const sf::RenderWindow& window,
	const sf::Event& event,
	Game& game
) {
	if (!event.is<sf::Event::MouseButtonPressed>()) {
		return;
	}

	const sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
	const sf::Vector2f point{
		static_cast<float>(mouse_position.x),
		static_cast<float>(mouse_position.y)
	};

	if (contains(point)) {
		game.append_buttons_clicked(this);
	}
}

int Button::get_id() const {
	return id;
}

int Button::get_column() const {
	return id % 8;
}

int Button::get_row() const {
	return id / 8;
}

sf::Vector2f Button::get_position() const {
	return position;
}

sf::Vector2f Button::get_size() const {
	return size;
}

const std::string& Button::get_text() const {
	return button_text;
}

bool Button::contains(sf::Vector2f point) const {
	return point.x >= position.x
		&& point.x <= position.x + size.x
		&& point.y >= position.y
		&& point.y <= position.y + size.y;
}

SquareButton::SquareButton(
	sf::Vector2f position,
	sf::Vector2f size,
	int row,
	int column,
	int id
) : Button(position, size, id), column(column), row(row) {}

int SquareButton::get_row() const {
	return row;
}

int SquareButton::get_column() const {
	return column;
}

MenuButton::MenuButton(
	sf::Vector2f position,
	sf::Vector2f size,
	int id,
	MenuOption menu_option
) : Button(position, size, id, menu_option_to_string(menu_option)),
	menu_option(menu_option) {}

MenuOption MenuButton::get_menu_option() const {
	return menu_option;
}
