#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include <cstdlib>
#include <time.h>

//#include <Windows.h>
//#include <filesystem>

using namespace sf;

int main()
{
    //std::cout << std::filesystem::current_path() << std::endl;
    //system("pause");
    RenderWindow window(VideoMode({ 1500,750 }), "dragon shooter!");

    Game game(&window);

    srand(time(NULL));
    //Game loop
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
        }

        game.Update();
        game.Draw();

    }
}
