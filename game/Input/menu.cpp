#include "menu.h"
#include "buttons.h"
#include "game.h"
#include "inputer.h"
#include "../Pieces/piece.h"
#include "../output/renderer.h"

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


MainMenu::MainMenu() {
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f{250.f, 210.f}, sf::Vector2f{300.f, 65.f}, 0, "Singleplayer"));
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f{250.f, 300.f}, sf::Vector2f{300.f, 65.f}, 1, "Multiplayer"));
}

std::optional<GameMode> MainMenu::show() {
	sf::RenderWindow window(sf::VideoMode({800, 550}), "Genialky");
	Renderer renderer(window, false);
	Inputer inputer(window);

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

		renderer.render_background();
		renderer.render(this);
		window.display();
	}

	return std::nullopt;
}

