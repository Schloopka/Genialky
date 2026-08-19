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

class PieceMenu : public Menu {
private:
    Piece& piece;
    std::vector<MenuOption> options;
public:
    PieceMenu(Piece& piece, Game& game);

    void process_clicks(const sf::RenderWindow& window, const sf::Event& event, Game& game);
};

class MainMenu : public Menu {
public:
    MainMenu();
    std::optional<GameMode> show();
};
