#include "buttons.h"
#include "game.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


class Game;

Button::Button(sf::Vector2f position, sf::Vector2f size, int id) {

	button_shape.setPosition(position);
	button_shape.setSize(size);
	button_shape.setFillColor(sf::Color::Blue);
	this->id = id;

}

void Button::draw(sf::RenderWindow& window) {

}
void Button::isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game) {
	if (event.is<sf::Event::MouseButtonPressed>()) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (button_shape.getGlobalBounds().contains(mousePosF)) {
			game.handle_events(this);

		}
	}
}

int Button::get_id() {
	return this->id;
}


SquareButton::SquareButton(sf::Vector2f position, sf::Vector2f size, int row, int column, int id)
	: row(row), column(column) 
{
	button_shape.setPosition(position);
	button_shape.setSize(size);
	button_shape.setFillColor(sf::Color::White);
	button_shape.setOutlineColor(sf::Color::Black);
	button_shape.setOutlineThickness(1.f);
	this->id = id;
}



sf::RectangleShape Button::get_button_shape() {
	return button_shape;
}
