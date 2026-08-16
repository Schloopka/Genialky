#include "client.h"
#include "game/game.h"
#include "server.h"

Client::Client(Server& server, bool is_white):window(sf::VideoMode({ 800, 550 }), "Genialky"),
 _renderer(window, !is_white), _inputer(window), server(server)
{
	this->is_white = is_white;
}

void Client::setup() {
	_renderer.render_background();
	make_buttons();
	_renderer.setup_texts();
	auto context = make_event_context();
	_renderer.render_game(server.get_game(), context);
}
void Client::render(){
	auto context = make_event_context();
	_renderer.render_game(server.get_game(), context);
}
void Client::set_message_for_user(std::string message) {
	this->action_description = message;
}

void Client::make_buttons() {
	for (int c = 0; c < 8; c++) {
		for (int r = 0; r < 8; r++) {
			const int displayed_row = is_white ? r : 7 - r;
			const int displayed_column = is_white ? c : 7 - c;
			this->buttons.emplace_back(std::make_unique<SquareButton>(
				sf::Vector2f(50.f + 100.f * displayed_column, 750.f - 100.f * displayed_row),
				sf::Vector2f(100.f, 100.f),
				r, c, 8 * r + c));
		}
	}

	this->buttons.emplace_back(std::make_unique<SquareButton>(sf::Vector2f(900.f, 500.f), sf::Vector2f(100.f, 100.f),
													 8, 0, 65));
	this->buttons.emplace_back(std::make_unique<SquareButton>(sf::Vector2f(900.f, 300.f), sf::Vector2f(100.f, 100.f),
													 8, 1, 66));
	std::string text = "End turn";
	this->buttons.emplace_back(std::make_unique<Button>(sf::Vector2(900.f, 425.f), sf::Vector2f(150.f, 50.f),
													 67, text,
												 [this](Game& game) {
											 auto context = make_event_context();
											 game.try_end_turn(context);
												 }));
}

void Client::resize_buttons(sf::Vector2u window_size) {
	constexpr float original_width = 1200.f;
	constexpr float original_height = 1000.f;

	if (window_size.x == 0 || window_size.y == 0) {
		return;
	}

	window.setView(sf::View(sf::FloatRect(
		{0.f, 0.f},
		{static_cast<float>(window_size.x), static_cast<float>(window_size.y)}
	)));

	const float scale_x = static_cast<float>(window_size.x) / original_width;
	const float scale_y = static_cast<float>(window_size.y) / original_height;

	for (auto& button : buttons) {
		sf::Vector2f position;
		sf::Vector2f size;

		if (button->get_id() < 64) {
			const int displayed_row = is_white ? button->get_row() : 7 - button->get_row();
			const int displayed_column = is_white ? button->get_column() : 7 - button->get_column();
			position = {
				(50.f + 100.f * displayed_column) * scale_x,
				(750.f - 100.f * displayed_row) * scale_y
			};
			size = {100.f * scale_x, 100.f * scale_y};
		} else if (button->get_id() == 65 || button->get_id() == 66) {
			position = {
				900.f * scale_x,
				(button->get_id() == 65 ? 500.f : 300.f) * scale_y
			};
			size = {100.f * scale_x, 100.f * scale_y};
		} else {
			position = {900.f * scale_x, 425.f * scale_y};
			size = {150.f * scale_x, 50.f * scale_y};
		}

		button->set_geometry(position, size);
	}

	if (activeMenu != nullptr) {
		activeMenu->resize_buttons(window_size);
	}
}

void Client::append_buttons_clicked(Button* button) {
	this->last_clicked.emplace_back(button);
}

void Client::clear_buttons_clicked() {
	this->last_clicked.clear();
}

void Client::process_input() {
	const InputAction action = _inputer.poll(get_buttons(), activeMenu.get());
	switch (action.type) {
		case InputActionType::CloseWindow:
			open = false;
			window.close();
			break;
		case InputActionType::WindowResized:
			resize_buttons(action.window_size);
			break;
		case InputActionType::ButtonClicked:
			if (action.button->has_onlclick()) {
				action.button->click(server.get_game());
			} else {
				last_clicked.emplace_back(action.button);
				auto context = make_event_context();
				server.get_game().handle_events(context);
			}
			break;
		case InputActionType::MenuButtonClicked:
			activeMenu->handle_events(*action.menu_button, server.get_game());
			activeMenu = nullptr;
			break;
		case InputActionType::None:
			break;
		default:
			break;
	}
}

GameEventContext Client::make_event_context() {
	return {is_white, last_clicked, action_description, activeMenu,
		possible_actions, get_buttons(), window.getSize()};
}


std::vector<Piece*> Client::get_pieces() const {
	return pieces;
}

const std::vector<Button*> Client::get_buttons() const {
	std::vector<Button*> result;
	result.reserve(buttons.size());

	for (const auto& button : buttons) {
		result.emplace_back(button.get());
	}

	return result;
}

Button* Client::get_button(int row, int column) const {
	for (const auto& button : buttons) {
		if (button->get_row() == row && button->get_column() == column) {
			return button.get();
		}
	}

	return nullptr;
}




const std::vector<Button*> Client::get_last_clicked_buttons() const {
	std::vector<Button*> result;
	result.reserve(last_clicked.size());

	for (const auto& button : last_clicked) {
		result.emplace_back(button);
	}

	return result;
}

const std::string& Client::get_action_descrtiption() const {
	return action_description;
}

const Menu* Client::get_menu() const {
	return activeMenu.get();
}

void Client::set_to_delete_menu(bool val) {
	this->to_delete_menu = val;
}

bool Client::is_open(){
	return this->open;
}
