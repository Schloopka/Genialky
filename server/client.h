#pragma once
#ifndef CLIENT_H
#define CLIENT_H


#include <SFML/Window.hpp>

#include "output/renderer.h"
#include "Input/inputer.h"
#include "Input/game_event_context.h"

class Server;

class Client {
public:
	Client(Server& server, bool is_singleplayer, bool is_white);
	Client(Server& server, bool is_singleplayer);

	virtual void setup() = 0; //calls functions to set up game, pieces and render background with fields for texts
	virtual void render() = 0;
	void make_buttons(); //make buttons which are permament (squares buttons, end turn button), put them into vector
	void resize_buttons(sf::Vector2u window_size); //resizes and repositions buttons to match the window
	void set_message_for_user(std::string message); //changes the text of message_for_user text field
	
	virtual void process_input() = 0;
	GameEventContext make_event_context(GameActionType action_type);
	GameEventContext make_event_context();

	void clear_buttons_clicked(); //clears the vector of last clicked buttons after action with them is made
	void append_buttons_clicked(Button* button);

	void set_to_delete_menu(bool val);

	std::vector<Piece*> get_pieces() const;
	const std::vector<Button*> get_buttons() const;
	Button* get_button(int row, int column) const;
	const std::vector<Button*> get_possible_actions() const;
	const std::vector<Button*> get_last_clicked_buttons() const;
	const Menu* get_menu() const;
	const std::string& get_action_descrtiption() const;

	bool is_open();
protected:
	Server& server;
	sf::RenderWindow window;
	Renderer _renderer;
	Inputer _inputer;

	std::string action_description;

	std::vector<std::unique_ptr<Button>> buttons; //vector of all buttons except menu buttons
	std::vector<Button*> last_clicked; //vector of last clicked buttons
	std::vector<Button*> possible_actions;
	std::unique_ptr<Menu> activeMenu; //displayed menu when player has to choose from more options
	std::vector<Piece*> pieces; //vector of all pieces on board
	
	MenuType menu_type = MenuType::NONE;

	
	bool to_delete_menu = false; //bool to remember if I want to delete a menu after the process of the menu is done
	
	bool is_white = true; //default view from white side
	bool is_singleplayer;
	bool open = true;
};

class SinglePlayerClient : public Client {
public:
	SinglePlayerClient(Server& server);

	void setup() override; //calls functions to set up game, pieces and render background with fields for texts
	void render() override;
	void process_input() override;
};

class MultiPlayerClient : public Client {
public:
	MultiPlayerClient(Server& server, bool is_white);

	void setup() override; //calls functions to set up game, pieces and render background with fields for texts
	void render() override;
	void process_input() override;
};


#endif
