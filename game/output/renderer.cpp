#include "renderer.h"

#include "../game.h"
#include "../Input/buttons.h"
#include "../Input/menu.h"
#include "../Input/game_event_context.h"
#include "../Pieces/piece.h"
#include "../Pieces/queen.h"

#include <algorithm>
#include <utility>

namespace {
constexpr float original_width = 1200.f;
constexpr float original_height = 1000.f;

sf::Vector2f get_scale(const sf::RenderWindow& window) {
    const sf::Vector2u size = window.getSize();
    return {
        static_cast<float>(size.x) / original_width,
        static_cast<float>(size.y) / original_height
    };
}

unsigned int scaled_text_size(unsigned int original_size, float scale) {
    return std::max(1u, static_cast<unsigned int>(original_size * scale));
}
}

Renderer::Renderer(sf::RenderWindow& window, bool black_perspective) : window(window), 
                                            action_description(this->font), who_is_on_move(this->font) {
    setup_textures();
    setup_texts();
}

void Renderer::render_background() {
    window.clear(background);
}

void Renderer::render_game(const Game& game, const GameEventContext& context) {
    black_perspective = !context.is_input_player_white;
    render_background();
    render_buttons(context.buttons, context.last_clicked, context.possible_actions);
    render(context.menu.get());
    render_pieces(game.get_pieces());
    render_texts(context.action_description, game.get_gamestate());
    window.display();
}

void Renderer::render_buttons(const std::vector<Button*>& buttons, 
                            const std::vector<Button*>& last_clicked,
                            const std::vector<Button*>& possible_actions) {
    for (const Button* button : buttons) {
        render(*button, false);
    }
    for (const Button* button : last_clicked) {
        render(*button, true);
    }
    for (const Button* button : possible_actions) {
        if (button == nullptr) {
            continue;
        }

        const sf::Vector2f position = button->get_position();
        const sf::Vector2f size = button->get_size();
        const float marker_size = std::min(size.x, size.y) * 0.2f;

        sf::ConvexShape marker(3);
        marker.setFillColor(sf::Color::Green);

        marker.setPoint(0, position);
        marker.setPoint(1, {position.x + marker_size, position.y});
        marker.setPoint(2, {position.x, position.y + marker_size});
        window.draw(marker);

        marker.setPoint(0, {position.x + size.x, position.y});
        marker.setPoint(1, {position.x + size.x - marker_size, position.y});
        marker.setPoint(2, {position.x + size.x, position.y + marker_size});
        window.draw(marker);

        marker.setPoint(0, {position.x, position.y + size.y});
        marker.setPoint(1, {position.x + marker_size, position.y + size.y});
        marker.setPoint(2, {position.x, position.y + size.y - marker_size});
        window.draw(marker);

        marker.setPoint(0, {position.x + size.x, position.y + size.y});
        marker.setPoint(1, {position.x + size.x - marker_size, position.y + size.y});
        marker.setPoint(2, {position.x + size.x, position.y + size.y - marker_size});
        window.draw(marker);
    }
}

void Renderer::render(const Menu* menu) {
    if (menu == nullptr) {
        return;
    }

    for (const MenuButton* button : menu->get_buttons()) {
        render(*button, false);
    }
}

void Renderer::render(const Button& button, bool is_clicked) {
    sf::RectangleShape shape(button.get_size());
    shape.setPosition(button.get_position());

    const auto* square_button = dynamic_cast<const SquareButton*>(&button);
    const auto* menu_button = dynamic_cast<const MenuButton*>(&button);

    if (square_button != nullptr) {
        const sf::Color dark_square(181, 136, 99);
        const sf::Color light_square(248, 219, 161);
        const sf::Color green_square(144, 238, 144);
        const bool is_dark =
            (square_button->get_row() + square_button->get_column()) % 2 == 1;
        shape.setFillColor(is_clicked ? green_square :is_dark ? dark_square : light_square);
    } else {
        shape.setFillColor(sf::Color::Blue);
    }

    window.draw(shape);

    if (button.get_text().empty()) {
        return;
    }

    sf::Text text(font);
    text.setString(button.get_text());
    text.setFillColor(sf::Color::Black);
    const sf::Vector2f scale = get_scale(window);
    text.setCharacterSize(scaled_text_size(25, std::min(scale.x, scale.y)));

    const sf::Vector2f position = button.get_position();
    const sf::Vector2f size = button.get_size();
    if (menu_button != nullptr) {
        text.setPosition({
            position.x + size.x / 10.f,
            position.y + size.y / 6.f
        });
    } else {
        text.setPosition({
            position.x + size.x / 5.f,
            position.y + size.y / 5.f
        });
    }

    window.draw(text);
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
    const sf::Vector2f scale = get_scale(window);
    const int displayed_row = black_perspective ? 7 - piece.get_row() : piece.get_row();
    const int displayed_column = black_perspective ? 7 - piece.get_column() : piece.get_column();
    sprite.setPosition({
        (42.f + displayed_column * 100.f) * scale.x,
        (740.f - displayed_row * 100.f) * scale.y
    });
    sprite.setScale({0.9f * scale.x, 0.9f * scale.y});
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
    const sf::Vector2f scale = get_scale(window);

    if (phase == airStrikePhase::NOT_ACTIVE) {
        const int displayed_row = black_perspective ? 7 - queen.get_row() : queen.get_row();
        const int displayed_column = black_perspective ? 7 - queen.get_column() : queen.get_column();
        sprite.setPosition({
            (42.f + displayed_column * 100.f) * scale.x,
            (740.f - displayed_row * 100.f) * scale.y
        });
    } else if (queen.is_piece_white()) {
        sprite.setPosition({893.f * scale.x, 490.f * scale.y});
    } else {
        sprite.setPosition({893.f * scale.x, 290.f * scale.y});
    }

    sprite.setScale({0.9f * scale.x, 0.9f * scale.y});
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

void Renderer::setup_texts(){
    const std::string path = "Textures and fonts/arial1.ttf";
    if (!font.openFromFile(path)){
        throw std::runtime_error("Failed to load font: " + path);
    }
    action_description.setString("Welcome to the game, white on turn");
	who_is_on_move.setString("White on move");
    action_description.setCharacterSize(30);
	action_description.setPosition({ 200.f,900.f });
	action_description.setFillColor(sf::Color::Black);
	who_is_on_move.setCharacterSize(30);
	who_is_on_move.setPosition({ 800.f,900.f });
	who_is_on_move.setFillColor(sf::Color::Black);
}
void Renderer::render_texts(std::string s_action_description, ON_TURN gamestate){
    const sf::Vector2f scale = get_scale(window);
    this->action_description.setString(s_action_description);
    this->who_is_on_move.setString(gamestate == ON_TURN::WHITE_TURN ? "White on move" : "Black on move");
    action_description.setPosition({200.f * scale.x, 900.f * scale.y});
    who_is_on_move.setPosition({800.f * scale.x, 900.f * scale.y});
    const unsigned int character_size = scaled_text_size(30, std::min(scale.x, scale.y));
    action_description.setCharacterSize(character_size);
    who_is_on_move.setCharacterSize(character_size);
    window.draw(this->action_description);
	window.draw(this->who_is_on_move);
}
