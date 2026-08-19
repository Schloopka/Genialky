#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <optional>
#include <vector>


class MenuButton;
class Button;
class Piece;
class Game;
enum class MenuOption;

enum class GameMode {
    Singleplayer,
    Multiplayer
};

class Menu {
protected:
    std::vector<std::unique_ptr<Button>> buttons;
public:
    Menu();
    virtual ~Menu();

    std::vector<Button*> get_buttons() const;
	void resize_buttons(sf::Vector2u window_size);

};

class IngameMenu : public Menu {
private:
    std::vector<MenuOption> options;
public:
    IngameMenu(std::vector<MenuOption> options, sf::Vector2u window_size);
};

class MainMenu : public Menu {
public:
    MainMenu();
    void resize_buttons(sf::Vector2u window_size);
    std::optional<GameMode> loop();
};
