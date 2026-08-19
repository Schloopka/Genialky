#include "menu.h"
#include "buttons.h"
#include "game.h"
#include "inputer.h"
#include "../Pieces/piece.h"

#include <algorithm>
#include <stdexcept>

Menu::Menu(){}

Menu::~Menu() = default;

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


IngameMenu::IngameMenu(std::vector<MenuOption> options, sf::Vector2u window_size){
	this->options = options;
	int i = 0;
	for (auto& option : options) {
		buttons.push_back(std::make_unique<MenuButton>(sf::Vector2f({ 1000.f, 100.f + (i)*100.f}), 
										sf::Vector2f({150.f, 50.f}), i, option));
		i++;
	}
	resize_buttons(window_size);
}


MainMenu::MainMenu() {
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f{250.f, 210.f}, sf::Vector2f{300.f, 65.f}, 0, "Singleplayer"));
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f{250.f, 300.f}, sf::Vector2f{300.f, 65.f}, 1, "Multiplayer"));
}

void MainMenu::resize_buttons(sf::Vector2u window_size) {
	const float scale_x = static_cast<float>(window_size.x) / 800.f;
	const float scale_y = static_cast<float>(window_size.y) / 550.f;

	buttons[0]->set_geometry({250.f * scale_x, 210.f * scale_y},
		{300.f * scale_x, 65.f * scale_y});
	buttons[1]->set_geometry({250.f * scale_x, 300.f * scale_y},
		{300.f * scale_x, 65.f * scale_y});
}

std::optional<GameMode> MainMenu::loop() {
	sf::RenderWindow window(sf::VideoMode({800, 550}), "Genialky");
	Inputer inputer(window);
	sf::Font font;
	if (!font.openFromFile("Textures and fonts/arial1.ttf")) {
		throw std::runtime_error("Failed to load menu font");
	}

	while (window.isOpen()) {
		const InputAction action = inputer.poll(get_buttons(), nullptr);
		switch (action.type) {
			case InputActionType::CloseWindow:
				window.close();
				break;
			case InputActionType::WindowResized:
				resize_buttons(action.window_size);
				break;
			case InputActionType::ButtonClicked:
				return action.button->get_id() == 0
					? GameMode::Singleplayer : GameMode::Multiplayer;
			default:
				break;
		}

		window.clear(sf::Color(35, 39, 47));
		const float text_scale = std::min(
			static_cast<float>(window.getSize().x) / 800.f,
			static_cast<float>(window.getSize().y) / 550.f);

		for (Button* menu_button : get_buttons()) {
			sf::RectangleShape shape(menu_button->get_size());
			shape.setPosition(menu_button->get_position());
			shape.setFillColor(sf::Color(55, 105, 185));
			window.draw(shape);

			sf::Text label(font, menu_button->get_text(),
				std::max(1u, static_cast<unsigned int>(28.f * text_scale)));
			label.setPosition(menu_button->get_position() + sf::Vector2f{
				55.f * static_cast<float>(window.getSize().x) / 800.f,
				14.f * static_cast<float>(window.getSize().y) / 550.f});
			window.draw(label);
		}
		window.display();
	}

	return std::nullopt;
}
