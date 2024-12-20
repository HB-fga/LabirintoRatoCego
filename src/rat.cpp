#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int xv, int yv, const std::string& textureFilename)
        : x(xv), y(yv), last_ticks(0), current_movement_index(0), direction(0), is_flip(false), textureFilename(textureFilename), dest_x(xv), dest_y(yv), moving(false), progress(0), frame(0) {
    }

    void Rat::update(unsigned ticks)
    {
        if (current_movement_index < movements.size()) {
            if (!last_ticks) {
                last_ticks = ticks;
            } else if (ticks - last_ticks > 300) {
                if(not moving)
                {
                    
                    current_movement_index++;
                    if(current_movement_index == movements.size()) return;
                    x = dest_x;
                    y = dest_y;
                    dest_x = movements[current_movement_index].first;
                    dest_y = movements[current_movement_index].second;
                    is_flip = (is_flip) ? false : true;

                    // Calcular a direção com base nas coordenadas
                    int delta_x = dest_x - x;
                    int delta_y = dest_y - y;
                    if (delta_x > 0) {
                        direction = 270; // Direita
                    } else if (delta_x < 0) {
                        direction = 90; // Esquerda
                    } else if (delta_y > 0) {
                        direction = 0; // Baixo
                    } else if (delta_y < 0) {
                        direction = 180; // Cima
                    }

                    moving = true;
                    progress = 0;
                }
                else
                {
                    constexpr double step = 0.25;
                    progress = step+progress;
                    if(rand()%4 == 0) frame = (frame+1)%2;
                    moving = progress <= 1.0;
                }
                last_ticks = ticks;
            }
        }
    }

    void Rat::reverseUpdate(unsigned ticks)
    {
        if (current_movement_index >= 0) {
            if (!last_ticks) {
                last_ticks = ticks;
            } else if (ticks - last_ticks > 1000) {
                int new_x = movements[current_movement_index].first;
                int new_y = movements[current_movement_index].second;
                is_flip = (is_flip) ? false : true;

                // Calcular a direção com base nas coordenadas
                int delta_x = new_x - x;
                int delta_y = new_y - y;
                if (delta_x < 0) {
                    direction = 270; // Direita
                } else if (delta_x > 0) {
                    direction = 90; // Esquerda
                } else if (delta_y < 0) {
                    direction = 0; // Baixo
                } else if (delta_y > 0) {
                    direction = 180; // Cima
                }

                x = new_x;
                y = new_y;

                last_ticks = ticks;
                current_movement_index = std::max(0, current_movement_index - 1);
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
        int ratSize = 57;

        SDL_Rect gSpriteClips[8];
        gSpriteClips[ 0 ].x = ratSize*frame;
        gSpriteClips[ 0 ].y = 0;
        gSpriteClips[ 0 ].w = ratSize;
        gSpriteClips[ 0 ].h = ratSize;

        gSpriteClips[ 1 ].x = ratSize*frame;
        gSpriteClips[ 1 ].y = ratSize*1;
        gSpriteClips[ 1 ].w = ratSize;
        gSpriteClips[ 1 ].h = ratSize;
        
        gSpriteClips[ 2 ].x = ratSize*frame;
        gSpriteClips[ 2 ].y = ratSize*2;
        gSpriteClips[ 2 ].w = ratSize;
        gSpriteClips[ 2 ].h = ratSize;

        gSpriteClips[ 3 ].x = ratSize*frame;
        gSpriteClips[ 3 ].y = ratSize*3;
        gSpriteClips[ 3 ].w = ratSize;
        gSpriteClips[ 3 ].h = ratSize;

        const int ratWidth{ ratSize }, ratHeight{ ratSize };
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        auto ratTexture = engine::loadTexture(textureFilename);

        SDL_Rect destRect{ xpos, ypos, ratWidth, ratHeight };
        SDL_Point center{ ratWidth / 2, ratHeight / 2 };
        SDL_RenderCopy(engine::getRenderer(), ratTexture.get(), &gSpriteClips[ direction/90 ], &destRect);
    }
}
