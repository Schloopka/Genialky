#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>


#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../gamestate.h"

class Button;
class Game;
class Menu;
class Piece;
class Queen;

struct PieceVisualKey {
    PieceType type;
    bool is_white;

    bool operator==(const PieceVisualKey& other) const noexcept {
        return type == other.type && is_white == other.is_white;
    }
};

struct PieceVisualKeyHash {
    std::size_t operator()(const PieceVisualKey& key) const noexcept {
        const auto type_hash = std::hash<PieceType>{}(key.type);
        const auto color_hash = std::hash<bool>{}(key.is_white);
        return type_hash ^ (color_hash << 1);
    }
};

class Renderer {
private:
    sf::RenderWindow& window;
    const sf::Color background = sf::Color(180, 180, 180);

    std::unordered_map<PieceVisualKey, sf::Texture, PieceVisualKeyHash> piece_textures;
    void setup_textures();
    void load_texture(PieceType type, bool is_white, const std::string& filename);

    sf::Font font;
    sf::Text action_description;
    sf::Text who_is_on_move;

public:
    explicit Renderer(sf::RenderWindow& window);

    void render_background();
    void render_game(const Game& game);
    void render_buttons(const std::vector<Button*>& buttons, const std::vector<Button*>& last_clicked);
    void render(const Button& button, bool is_clicked);
    void render(const Menu* menu);
    void render_pieces(const std::vector<Piece*>& pieces);
    void render(Piece& piece);
    void render(Queen& queen);
    void setup_texts();
    void render_texts(std::string action_description, Gamestate gamestate);
};

#endif
