#include <iostream>
#include "Game.hpp"


void launch_game(int ac, char **av)
{
    Game *game = Game::getInstance();

    if (ac >= 2)
    {
        if (std::string(av[1]) == "--nosound")
            game->setSound(false);
    }

    while (game->isRunning())
    {
        game->pollEvents();
        game->display();
    }
    delete game;
}


int main(int ac, char **av)
{
    try
    {
        launch_game(ac, av);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
