#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


class MenuButton;
class Piece;
class Game;
enum class MenuOption;

class Menu {
protected:
    std::vector<MenuButton*> buttons;
public:
    Menu();
    virtual void process_clicks(const sf::RenderWindow& window, const sf::Event& event, Game& game) = 0;
    virtual ~Menu() = default;

    const std::vector<MenuButton*>& get_buttons() const;

};

class PieceMenu : public Menu {
private:
    Piece& piece;
    std::vector<MenuOption> options;
public:
    PieceMenu(Piece& piece, Game& game);

    void process_clicks(const sf::RenderWindow& window, const sf::Event& event, Game& game) override;
    void handle_events(MenuButton& button, Game& game);
};