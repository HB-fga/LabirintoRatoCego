#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv) : x(xv), y(yv) {}

    void Rat::update(unsigned ticks)
    {
        
    }

    void Rat::draw(int xpos, int ypos) const
    {
        // The width and height of the rat
        const int ratWidth{ 71 }, ratHeight{ 58 };
        
        // Load the rat texture using the engine's loadTexture function
        std::shared_ptr<SDL_Texture> ratTexture = engine::loadTexture("E:/TCC/LabirintoRatoCego/assets/rat/rat.png");
        if (!ratTexture) {
            // Handle the case where loading the texture failed
            return;
        }

        // Set the position where the rat will be drawn
        SDL_Rect destRect{ xpos, ypos, ratWidth, ratHeight };

        SDL_Rect srcRect{ 0, ratHeight, ratWidth, ratHeight };
        srcRect.y = 2 * ratHeight;

        // Draw the rat texture on the renderer
        SDL_RenderCopy(engine::getRenderer(), ratTexture.get(), &srcRect, &destRect);
    }

}