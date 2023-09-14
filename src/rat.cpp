#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv) : x(xv), y(yv), last_ticks(0) {
    }

    void Rat::update(unsigned ticks)
    {
        if (not last_ticks) {
            last_ticks = ticks;
        }
        else if (ticks - last_ticks > 1000) {
            x++;
            last_ticks = ticks;
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
