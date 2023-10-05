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
        maze = game::Maze::fromFile("./assets/maps/map.txt", "./assets/movements/rat.txt");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

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
        maze.draw(60, 90);


        // if (loops == 400)
        //     quit = true;
        // loops++;

        engine::screen::show();
    }

    engine::close();

    return 0;
}
