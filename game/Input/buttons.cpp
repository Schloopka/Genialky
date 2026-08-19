#include "buttons.h"

Button::Button(
	sf::Vector2f position,
	sf::Vector2f size,
	int id,
	std::string text,
	GameActionType action_type
) : position(position), size(size), button_text(std::move(text)), 
	id(id), action_type(action_type) {}

GameActionType Button::get_action_type() const{
	return action_type;
}

bool Button::has_onclick() const{
	if (action_type == GameActionType::END_TURN_MOVE){
		return true;
	}
	return false;
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

void Button::set_geometry(sf::Vector2f new_position, sf::Vector2f new_size) {
	position = new_position;
	size = new_size;
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
