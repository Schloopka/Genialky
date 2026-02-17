#include "buttons.h"
#include "game.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


class Game;

Button::Button(sf::Vector2f position, sf::Vector2f size, int id,  sf::Font& font, const std::string& text="") : button_text(font) {

	this->button_shape.setPosition(position);
	this->button_shape.setSize(size);
	this->button_shape.setFillColor(sf::Color::Blue);
	this->button_text.setFont(font);
	this->button_text.setString(text);
	this->button_text.setPosition({ position.x + size.x / 5.f, position.y + size.y / 5.f });
	this->button_text.setFillColor(sf::Color::Black);
	this->button_text.setCharacterSize(25);

	this->id = id;

}
/*Button::Button(sf::Vector2f position, sf::Vector2f size, int id) :button_text(font)
{
	button_shape.setPosition(position);
	button_shape.setSize(size);
	this->id = id;
}*/
void Button::draw_button(sf::RenderWindow& window) {
	window.draw(button_shape);
	window.draw(button_text);
}
void Button::isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game) {
	if (event.is<sf::Event::MouseButtonPressed>()) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (button_shape.getGlobalBounds().contains(mousePosF)) {
			game.append_buttons_clicked(this);
			/*game.handle_events();*/

		}
	}
}

int Button::get_id() {
	return this->id;
}
sf::RectangleShape Button::get_button_shape() {
	return button_shape;
}

SquareButton::SquareButton(sf::Vector2f position, sf::Vector2f size, int row, int column, int id, sf::Font& font)
	: Button(position, size, id, font), row(row), column(column)
{
	button_shape.setPosition(position);
	button_shape.setSize(size);
	//colours of squares
	sf::Color black(181, 136, 99);
	sf::Color white(248, 219, 161);
	if ((row % 2 + column % 2) % 2 == 1){
		button_shape.setFillColor(black);
	}
	else {
		button_shape.setFillColor(white);
	}
	this->id = id;
}

MenuButton::MenuButton(sf::Vector2f position, sf::Vector2f size, int id, sf::Font& font, MenuOption menu_option)
	:Button(position, size, id, font), menu_option(menu_option) {
	button_shape.setPosition(position);
	button_shape.setSize(size);
	this->button_shape.setFillColor(sf::Color::Blue);
	this->button_text.setFont(font);
	std::string button_text = menu_option_to_string(menu_option);
	this->button_text.setString(button_text);
	this->button_text.setPosition({ position.x + size.x / 10.f, position.y + size.y / 6.f });
	this->button_text.setFillColor(sf::Color::Black);
	this->button_text.setCharacterSize(25);

}

MenuOption MenuButton::get_menu_option() {
	return this->menu_option;
}




