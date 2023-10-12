#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv) : x(xv), y(yv), last_ticks(0), current_movement_index(0), direction(0), is_flip(false){
    }

    void Rat::update(unsigned ticks)
    {
        if (current_movement_index < (int) movements.size()) {
            if (not last_ticks) {
                last_ticks = ticks;
            } else if (ticks - last_ticks > 1000) {
                char movement = movements[current_movement_index];
                is_flip = (is_flip) ? false : true;
                // std::cout << "INDEX = " << current_movement_index << "\n";
                // std::cout << "flip = " << flip << "\n";
                if (movement == 'D') {
                    y++;
                    direction = 0;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                } else if (movement == 'L') {
                    x--;
                    direction = 90;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                } else if (movement == 'R') {
                    x++;
                    direction = 270;
                    // std::cout << "MOVIMENTO = " << movement << "\n";
                } else if (movement == 'U') {
                    y--;
                    direction = 180;
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
        const int ratWidth{ 40 }, ratHeight{ 48 };

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (is_flip) {
            flip = SDL_FLIP_HORIZONTAL;
        }

        // Load the mouse texture using the engine's loadTexture function
        auto ratTexture = engine::loadTexture("./assets/rat/whiteRat.png");
        if (!ratTexture) {
            return;
        }

        // Define the position where the mouse will be drawn
        SDL_Rect destRect{ xpos, ypos, ratWidth, ratHeight };

        SDL_Point center{ ratWidth / 2, ratHeight / 2 };

        SDL_RenderCopyEx(engine::getRenderer(), ratTexture.get(), nullptr, &destRect, direction, &center, flip);
    }

}
