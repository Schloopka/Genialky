// Genialky.cpp : 
#include <iostream>

#include "game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>


int main()
{
    
    Game game;

    game.setup();
    game.run();

    return 0;
    
}


