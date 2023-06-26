#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv) : x(xv), y(yv), last_ticks(-1) {
    }

    void Rat::update(unsigned ticks)
    {
        if (last_ticks == -1) {
            last_ticks = ticks;
        }
        else if (ticks - last_ticks > 1000) {
            if (x < 4 && y == 0)
            {
                x++;
                last_ticks = ticks;
            }
            else if (x == 4 && y == 0)
            {
                y++;
                last_ticks = ticks;
            }
            else if (x == 4 && y == 1)
            {
                x--;
                last_ticks = ticks;
            }
            else if (x == 3 && y == 1)
            {
                y++;
                last_ticks = ticks;
            }
            else if (x == 3 && y == 2)
            {
                x--;
                last_ticks = ticks;
            }
            else if (x == 3 && y == 2)
            {
                x--;
                last_ticks = ticks;
            }
            else if (x == 2 && y == 2)
            {
                y--;
                last_ticks = ticks;
            }
            else if ((x == 2 || x == 1) && y == 1)
            {
                x--;
                last_ticks = ticks;
            }
            else if (x == 0 && y == 1)
            {
                y++;
                last_ticks = ticks;
            }
            else if (x == 0 && y == 2)
            {
                x++;
                last_ticks = ticks;
            }
            else if (x == 1 && (y >= 2 && y < 4))
            {
                y++;
                last_ticks = ticks;
            }
            else if ((x >= 1 && x < 3) && y == 4)
            {
                x++;
                last_ticks = ticks;
            }
            else if (x == 3 && y == 4)
            {
                y--;
                last_ticks = ticks;
            }
            else if (x == 3 && y == 3)
            {
                x++;
                last_ticks = ticks;
            }
        }
    }

    void Rat::draw(int xpos, int ypos) const
    {
        // The width and height of the mouse
        const int ratWidth{ 71 }, ratHeight{ 58 };

        // Load the mouse texture using the engine's loadTexture function
        auto ratTexture = engine::loadTexture("./assets/rat/rat.png");
        if (!ratTexture) {
            return;
        }

        // Define the position where the mouse will be drawn
        SDL_Rect destRect{ xpos, ypos, ratWidth, ratHeight };

        SDL_Rect srcRect{ 0, ratHeight, ratWidth, ratHeight };
        srcRect.y = 2 * ratHeight;

        // Draw mouse texture in renderer
        SDL_RenderCopy(engine::getRenderer(), ratTexture.get(), &srcRect, &destRect);
    }

}
