#include "rat.h"
#include "maze.h"
#include "engine.h"
#include <SDL.h>

namespace game {

    Rat::Rat(int xv, int yv) : x(xv), y(yv) {
    }

    void Rat::update(unsigned ticks)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                // quit = true;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    y -= ticks;
                    break;

                case SDLK_DOWN:
                    y += ticks;
                    break;

                case SDLK_LEFT:
                    x -= ticks;
                    break;

                case SDLK_RIGHT:
                    x += ticks;
                    break;
                }
            }
        }
    }

    void Rat::draw(int xpos, int ypos) const
    {
        // The width and height of the mouse
        const int ratWidth{ 71 }, ratHeight{ 58 };

        // Load the mouse texture using the engine's loadTexture function
        std::shared_ptr<SDL_Texture> ratTexture = engine::loadTexture("E:/TCC/LabirintoRatoCego/assets/rat/ra.png");
        if (!ratTexture) {
            return false;
        }

        // Define the position where the mouse will be drawn
        SDL_Rect destRect{ xpos + x, ypos + y, ratWidth, ratHeight };

        SDL_Rect srcRect{ 0, ratHeight, ratWidth, ratHeight };
        srcRect.y = 2 * ratHeight;

        // Draw mouse texture in renderer
        SDL_RenderCopy(engine::getRenderer(), ratTexture.get(), &srcRect, &destRect);
    }

}
