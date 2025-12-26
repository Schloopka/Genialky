#pragma once



#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

class Game;

class Button {
public:
	Button(sf::Vector2f position, sf::Vector2f size, int id);
	
	Button() = default;
	virtual void isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game);
	void draw(sf::RenderWindow& window);
	int get_id();
	sf::RectangleShape get_button_shape();
protected: 
	sf::RectangleShape button_shape;
	int id = 0;

};

class SquareButton : public Button {
private:
	int column ;
	int row;
	
public:
	SquareButton(sf::Vector2f position, sf::Vector2f size, int row, int column, int id) ;
	/*void isClicked(const sf::RenderWindow& window, const sf::Event& event, Game& game) override;*/

};

