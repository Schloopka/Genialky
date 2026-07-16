#include "menu.h"
#include "buttons.h"
#include "game.h"
#include "../Pieces/piece.h"

Menu::Menu(){}

void Menu::render(sf::RenderWindow& window) {
	for (MenuButton* button : buttons) {
		button->draw_button(window);
	}
}



PieceMenu::PieceMenu(Piece& piece, Game& game):piece(piece){
	this->piece = piece;
	this->options = this->piece.get_menu_options(game);
	int i = 0;
	for (auto& option : options) {
		buttons.push_back(new MenuButton({ 1000.f, 100.f + (i)*100.f}, {150.f, 50.f}, i, game.font, option));
		i++;
	}
}

void PieceMenu::process_clicks(const sf::RenderWindow& window, const sf::Event& event, Game& game) {
	for (MenuButton* button : buttons) {
		if (event.is<sf::Event::MouseButtonPressed>()) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (button->get_button_shape().getGlobalBounds().contains(mousePosF)) {
				handle_events(*button, game);

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
	game.set_to_delete_menu(true);
}
void PieceMenu::render(sf::RenderWindow& window) {
	for (MenuButton* button : buttons) {
		button->draw_button(window);
	}
}