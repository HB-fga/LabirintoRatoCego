#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv) : x(xv), y(yv), last_ticks(0), current_movement_index(0), direction(0){
    }

    void Rat::update(unsigned ticks)
    {
        if (current_movement_index < movements.size()) {
            if (not last_ticks) {
                last_ticks = ticks;
            } else if (ticks - last_ticks > 1000) {
                char movement = movements[current_movement_index];
                // std::cout << "INDEX = " << current_movement_index << "\n";
                // std::cout << "MOVIMENTO = " << movement << "\n";
                if (movement == 'D') {
                    y++;
                    direction = 0;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                } else if (movement == 'L') {
                    x--;
                    direction = 1;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                } else if (movement == 'R') {
                    x++;
                    direction = 2;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                } else if (movement == 'U') {
                    y--;
                    direction = 3;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                }
                last_ticks = ticks;
                current_movement_index++;
            }
        }
    }

    void Rat::setMovements(const std::vector<char>& movements) {
        this->movements = movements;
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
        srcRect.y = direction * ratHeight;

        // Draw mouse texture in renderer
        SDL_RenderCopy(engine::getRenderer(), ratTexture.get(), &srcRect, &destRect);
    }

}
