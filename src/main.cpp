#include "engine.h"
#include "maze.h"
#include "rat.h"

int main(int, char* [])
{
    // Start up the engine
    if (not engine::init(640, 480))
    {
        engine::close();
        return -1;
    }

    // Start the game
    game::Maze maze;
    try
    {
        maze = game::Maze::fromFile("E:/TCC/LabirintoRatoCego/assets/maps/maze1.txt");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    game::Rat rat(0, 0);
    
    // Main loop
    bool quit = false;
    while (!quit)
    {
        engine::screen::clear();
        maze.draw(10, 10);
        rat.draw(10, 10);
		rat.update(75);
        engine::screen::show();
    }

    engine::close();

    return 0;
}
