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

	// Inicializa o jogo
	game::Maze maze(5, 5);

	// Main loop
	while (true)
	{
		// 1. process_events();


		// 2. update_objects();

	   // 3. draw the frame
		engine::screen::clear();

		maze.loadMazeFromFile("E:/TCC/LabirintoRatoCego/assets/maps/maze1.txt");
		maze.draw(10,10);
		//rat.draw();

		engine::screen::show();

		// 4. wait_until_next_frame();

		break;
	}

	
	engine::close();

	return 0;
}
