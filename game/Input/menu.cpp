#include "menu.h"
#include "buttons.h"
#include "game.h"
#include "../Pieces/piece.h"

#include <stdexcept>

Menu::Menu(){}

void Menu::handle_events(MenuButton&, Game&) {}

std::vector<Button*> Menu::get_buttons() const{
	std::vector<Button*> result;
	result.reserve(buttons.size());
	for (auto& button : buttons){
		result.push_back(button.get());
	}

	return result;
}

void Menu::resize_buttons(sf::Vector2u window_size) {
	const float scale_x = static_cast<float>(window_size.x) / 1200.f;
	const float scale_y = static_cast<float>(window_size.y) / 1000.f;

	for (std::size_t i = 0; i < buttons.size(); ++i) {
		buttons[i]->set_geometry(
			{1000.f * scale_x, (100.f + static_cast<float>(i) * 100.f) * scale_y},
			{150.f * scale_x, 50.f * scale_y}
		);
	}
}


PieceMenu::PieceMenu(Piece& piece, Game& game):piece(piece){
	this->piece = piece;
	this->options = this->piece.get_menu_options(game);
	int i = 0;
	for (auto& option : options) {
		buttons.push_back(std::make_unique<MenuButton>(sf::Vector2f({ 1000.f, 100.f + (i)*100.f}), 
										sf::Vector2f({150.f, 50.f}), i, option));
		i++;
	}
}

void PieceMenu::process_clicks(const sf::RenderWindow& window, const sf::Event& event, Game& game) {
	for (Button* base_button : get_buttons()) {
		MenuButton* button = static_cast<MenuButton*>(base_button);
		if (event.is<sf::Event::MouseButtonPressed>()) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (button->contains(mousePosF)) {
				this->handle_events(*button, game);

			}
		}
	}
}

MainMenu::MainMenu() {
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f{250.f, 210.f}, sf::Vector2f{300.f, 65.f}, 0, "Singleplayer"));
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f{250.f, 300.f}, sf::Vector2f{300.f, 65.f}, 1, "Multiplayer"));
}

std::optional<GameMode> MainMenu::show() {
	sf::RenderWindow window(sf::VideoMode({800, 550}), "Genialky");
	sf::Font font;
	if (!font.openFromFile("Textures and fonts/arial1.ttf")) {
		throw std::runtime_error("Failed to load menu font");
	}

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (event->is<sf::Event::MouseButtonPressed>()) {
				const sf::Vector2i mouse = sf::Mouse::getPosition(window);
				for (Button* button : get_buttons()) {
					if (button->contains({static_cast<float>(mouse.x), static_cast<float>(mouse.y)})) {
						return button->get_id() == 0
							? GameMode::Singleplayer : GameMode::Multiplayer;
					}
				}
			}
		}

		window.clear(sf::Color(35, 39, 47));
		for (Button* menu_button : get_buttons()) {
			sf::RectangleShape shape(menu_button->get_size());
			shape.setPosition(menu_button->get_position());
			shape.setFillColor(sf::Color(55, 105, 185));
			window.draw(shape);

			sf::Text text(font, menu_button->get_text(), 28);
			text.setPosition(menu_button->get_position() + sf::Vector2f{55.f, 14.f});
			window.draw(text);
		}
		window.display();
	}

	return std::nullopt;
}

void PieceMenu::handle_events(MenuButton& button, Game& game) {
	MenuOption option = button.get_menu_option();
	switch (option) {
	case MenuOption::DONT_MOVE:
		this->piece.set_moves_when_attack(false);
		this->piece.attack(game.get_after_menu_coordinates().first, game.get_after_menu_coordinates().second,
			game, this->piece.get_attack_type());
		break;

	case MenuOption::MOVE_TO_ATTACKED_SQUARE:
		this->piece.set_moves_when_attack(true);
		this->piece.attack(game.get_after_menu_coordinates().first, game.get_after_menu_coordinates().second,
			game, this->piece.get_attack_type());
		break;

	case MenuOption::BISHOP:
		game.promote_piece(piece, { game.get_after_menu_coordinates().first, game.get_after_menu_coordinates().second }, PieceType::BISHOP);
		break;
	case MenuOption::KNIGHT:
		game.promote_piece(piece, { game.get_after_menu_coordinates().first, game.get_after_menu_coordinates().second }, PieceType::KNIGHT);
		break;
	case MenuOption::ROOK:
		game.promote_piece(piece, { game.get_after_menu_coordinates().first, game.get_after_menu_coordinates().second }, PieceType::ROOK);
		break;
	case MenuOption::QUEEN:
		game.promote_piece(piece, { game.get_after_menu_coordinates().first, game.get_after_menu_coordinates().second }, PieceType::QUEEN);
		break;
			
	}
}
