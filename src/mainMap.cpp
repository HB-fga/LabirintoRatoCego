#include "mapRender.h"
#include "engine.h"

int main(int argc, char* args[]) {
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
    const int MAZE_WIDTH = 30;
    const int MAZE_HEIGHT = 15;

    if (!engine::init(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        std::cerr << "Failed to initialize the engine." << std::endl;
        return -1;
    }

    MapRenderer mapRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, MAZE_WIDTH, MAZE_HEIGHT);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                if (mouseX >= mapRenderer.buttonX && mouseX <= mapRenderer.buttonX + mapRenderer.buttonWidth &&
                    mouseY >= mapRenderer.buttonY && mouseY <= mapRenderer.buttonY + mapRenderer.buttonHeight) {
                    mapRenderer.saveMapToFile("./assets/maps/map.txt");
                } else {
                    mapRenderer.updateCellColor(e.button.x, e.button.y);
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        mapRenderer.generateMaze();
    }

    engine::close();
    return 0;
}
