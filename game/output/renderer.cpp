#include "renderer.h"
#include "../game.h"
#include "../Input/buttons.h"
#include "../Input/menu.h"

Renderer::Renderer(sf::RenderWindow& window):window(window){}

void Renderer::render_background(){
	window.clear(background);
}

void Renderer::render_game(const Game& game){
    render_background();
    render_buttons(game.get_buttons());
    render_menu(game.get_menu());

}

void Renderer::render_buttons(const std::vector<Button*>& buttons) {
	for (const Button* button : buttons) {
		draw_button(*button);
	}
	

}

void Renderer::render_menu(const Menu* menu){
    if (menu != nullptr) {
		for (MenuButton* button : menu->get_buttons()) {
		    draw_button(*button);
	}
	}
}

void Renderer::draw_button(const Button& button){
    window.draw(button.get_shape());
	window.draw(button.get_text());
}
