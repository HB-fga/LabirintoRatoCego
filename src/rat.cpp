#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv, const std::string& textureFilename)
        : x(xv), y(yv), last_ticks(0), current_movement_index(1), direction(0), is_flip(false), textureFilename(textureFilename) {
    }

    void Rat::update(unsigned ticks)
    {
        if (current_movement_index < movements.size()) {
            if (!last_ticks) {
                last_ticks = ticks;
            } else if (ticks - last_ticks > 1000) {
                int new_x = movements[current_movement_index].first;
                int new_y = movements[current_movement_index].second;
                is_flip = (is_flip) ? false : true;

                // Calcular a direção com base nas coordenadas
                int delta_x = new_x - x;
                int delta_y = new_y - y;
                if (delta_x > 0) {
                    direction = 270; // Direita
                } else if (delta_x < 0) {
                    direction = 90; // Esquerda
                } else if (delta_y > 0) {
                    direction = 0; // Baixo
                } else if (delta_y < 0) {
                    direction = 180; // Cima
                }

                x = new_x;
                y = new_y;

                last_ticks = ticks;
                current_movement_index++;
            }
        }
    }

    void Rat::setMovements(const std::vector<std::pair<int, int>>& movements) {
        this->movements = movements;
    }

    int Rat::getIndex() const
    {
        return current_movement_index;
    }

    void Rat::draw(int xpos, int ypos) const 
    {
        const int ratWidth{ 40 }, ratHeight{ 48 };
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (is_flip) {
            flip = SDL_FLIP_HORIZONTAL;
        }

        auto ratTexture = engine::loadTexture(textureFilename);
        if (!ratTexture) {
            return;
        }

        printf("%d, %d\n", xpos, ypos); 

        SDL_Rect destRect{ xpos, ypos, ratWidth, ratHeight };
        SDL_Point center{ ratWidth / 2, ratHeight / 2 };
        SDL_RenderCopyEx(engine::getRenderer(), ratTexture.get(), nullptr, &destRect, direction, &center, flip);
    }

}
