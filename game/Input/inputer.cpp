#include "inputer.h"

Inputer::Inputer(sf::RenderWindow& window) : window(window){}

InputAction Inputer::poll(const std::vector<Button*>& buttons, const Menu* active_menu){
    while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			return InputAction(InputActionType::CloseWindow);
		}
		if (const auto* resized = event->getIf<sf::Event::Resized>()) {
			return {InputActionType::WindowResized, resized->size};
		}
			if (event->is<sf::Event::MouseButtonPressed>()) {
	            const sf::Vector2i pixel_position = sf::Mouse::getPosition(window);
	            const sf::Vector2f position = window.mapPixelToCoords(pixel_position);
			//if there is an active menu, we dont want to allow any other clicks
			if (active_menu != nullptr) {
                if (MenuButton* button = find_clicked_menu_button(position, active_menu, *event)){
				//if menu is meant to be destroyed after something is clicked, 
                    return {InputActionType::MenuButtonClicked, nullptr, button};
                }
			}
			//handle square buttons
			else {
				if (Button* button = find_clicked_button(position, buttons, *event)){
				//if menu is meant to be destroyed after something is clicked, 
                    return {InputActionType::ButtonClicked, button, nullptr};
                }
			}
		}
	}
    return {};
}

MenuButton* Inputer::find_clicked_menu_button(sf::Vector2f position, const Menu* menu, const sf::Event& event){
	for (Button* base_button : menu->get_buttons()) {
		MenuButton* button = static_cast<MenuButton*>(base_button);
		if (event.is<sf::Event::MouseButtonPressed>()) {
			if (button->contains(position)) {
				return button;

			}
		}
	}
    return nullptr;
}

Button* Inputer::find_clicked_button(sf::Vector2f position, const std::vector<Button*>& buttons, const sf::Event& event){
    for (Button* button : buttons) {
		if (event.is<sf::Event::MouseButtonPressed>()) {
			if (button->contains(position)) {
				return button;
			}
		}
	}
    return nullptr;
}
