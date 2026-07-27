#include "renderer.h"

#include "../game.h"
#include "../Input/buttons.h"
#include "../Input/menu.h"
#include "../Pieces/piece.h"
#include "../Pieces/queen.h"

#include <stdexcept>
#include <utility>

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
    setup_textures();
}

void Renderer::render_background() {
    window.clear(background);
}

void Renderer::render_game(const Game& game) {
    render_background();
    render_buttons(game.get_buttons());
    render(game.get_menu());
    render_pieces(game.get_pieces());
}

void Renderer::render_buttons(const std::vector<Button*>& buttons) {
    for (const Button* button : buttons) {
        render(*button);
    }
}

void Renderer::render(const Menu* menu) {
    if (menu == nullptr) {
        return;
    }

    for (const MenuButton* button : menu->get_buttons()) {
        render(*button);
    }
}

void Renderer::render(const Button& button) {
    window.draw(button.get_shape());
    window.draw(button.get_text());
}

void Renderer::render_pieces(const std::vector<Piece*>& pieces) {
    for (Piece* piece : pieces) {
        render(*piece);
    }
}

void Renderer::render(Piece& piece) {
    if (piece.get_type() == PieceType::QUEEN) {
        render(static_cast<Queen&>(piece));
        return;
    }

    const PieceVisualKey key{piece.get_type(), piece.is_piece_white()};
    const auto texture = piece_textures.find(key);
    if (texture == piece_textures.end()) {
        return;
    }

    sf::Sprite sprite(texture->second);
    sprite.setPosition({
        42.f + piece.get_column() * 100.f,
        740.f - piece.get_row() * 100.f
    });
    sprite.setScale({0.9f, 0.9f});
    window.draw(sprite);
}

void Renderer::render(Queen& queen) {
    const PieceVisualKey key{PieceType::QUEEN, queen.is_piece_white()};
    const auto texture = piece_textures.find(key);
    if (texture == piece_textures.end()) {
        return;
    }

    sf::Sprite sprite(texture->second);
    const airStrikePhase phase = queen.get_air_strike_phase();

    if (phase == airStrikePhase::NOT_ACTIVE) {
        sprite.setPosition({
            42.f + queen.get_column() * 100.f,
            740.f - queen.get_row() * 100.f
        });
    } else if (queen.is_piece_white()) {
        sprite.setPosition({893.f, 490.f});
    } else {
        sprite.setPosition({893.f, 290.f});
    }

    sprite.setScale({0.9f, 0.9f});
    window.draw(sprite);
}

void Renderer::setup_textures() {
    load_texture(PieceType::PAWN, true, "white-pawn.png");
    load_texture(PieceType::PAWN, false, "black-pawn.png");
    load_texture(PieceType::BISHOP, true, "white-bishop.png");
    load_texture(PieceType::BISHOP, false, "black-bishop.png");
    load_texture(PieceType::KNIGHT, true, "white-knight.png");
    load_texture(PieceType::KNIGHT, false, "black-knight.png");
    load_texture(PieceType::ROOK, true, "white-rook.png");
    load_texture(PieceType::ROOK, false, "black-rook.png");
    load_texture(PieceType::QUEEN, true, "white-queen.png");
    load_texture(PieceType::QUEEN, false, "black-queen.png");
    load_texture(PieceType::KING, true, "white-king.png");
    load_texture(PieceType::KING, false, "black-king.png");
}

void Renderer::load_texture(
    PieceType type,
    bool is_white,
    const std::string& filename
) {
    sf::Texture texture;
    const std::string path = "Textures and fonts/" + filename;
    if (!texture.loadFromFile(path)) {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    piece_textures.emplace(PieceVisualKey{type, is_white}, std::move(texture));
}
