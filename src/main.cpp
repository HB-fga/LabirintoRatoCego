#include "engine.h"
#include "maze.h"
#include "rat.h"


int main(int, char* [])
{
    // Start up the engine
    if (not engine::init(1920, 1080))
    {
        engine::close();
        return -1;
    }

    // Start the game
    game::Maze maze;
    try
    {
        maze = game::Maze::loadMapfromFile("./assets/maps/map2.txt");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    std::vector<char> movements;
    try
    {
        movements = maze.loadMovementsFromFile("./assets/movements/rat.txt");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    maze.setMovements(movements);

    // Main loop
    bool quit = false;
    int loops = 1;

    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
               quit = true;  // Permite fechar o jogo ao pressionar 'esc'
               break;
           }
        }

        engine::screen::clear();
        maze.update(SDL_GetTicks());
        maze.drawCentered();

        engine::screen::show();
    }

    engine::close();

    return 0;
}
