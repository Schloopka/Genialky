#include "menu.h"
#include "buttons.h"
#include "game.h"
#include "../Pieces/piece.h"

Menu::Menu(){}

std::vector<MenuButton*> Menu::get_buttons() const{
	std::vector<MenuButton*> result;
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
	for (MenuButton* button : get_buttons()) {
		if (event.is<sf::Event::MouseButtonPressed>()) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (button->contains(mousePosF)) {
				this->handle_events(*button, game);

			}
		}
	}
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
