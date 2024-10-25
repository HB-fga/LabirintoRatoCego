#include "cell.h"
#include "engine.h"
#include <iostream>

namespace game {

    const int Cell::wall_thickness = 5;

    Cell::Cell(bool v, bool nv, bool d, bool maze_exit, bool maze_start, int cell_size)
        : not_valid(v), is_valid(nv), is_decision(d), maze_exit(maze_exit), maze_start(maze_start), m_cell_size(cell_size) {}

    void Cell::draw(int xpos, int ypos) const {
        SDL_Color greenColor { 0, 255, 0, 255 };
        SDL_Color blackColor { 0, 0, 0, 255 };
        SDL_Color grayDarkColor{ 64, 64, 64, 255 };
        SDL_Color grayLightColor{ 192, 192, 192, 255 };
        SDL_Color blueColor{ 0, 0, 255, 255 };

        if (not_valid)
        {
            renderTexture(xpos, ypos, "./assets/tiles/wall.png");
        } 
        else if (is_valid) 
        {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, grayLightColor);
        }
        else if (maze_exit) 
        {
            renderTexture(xpos, ypos,"./assets/tiles/end.png");
        } 
        else if (maze_start) 
        {
            renderTexture(xpos, ypos,"./assets/tiles/start.png");
        }
        else {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, grayLightColor);

            // Calcule o centro do quadrado
            int centerX = xpos + m_cell_size / 2;
            int centerY = ypos + m_cell_size / 2;

            int squareSize = m_cell_size / 4;  // Metade do tamanho da célula
            int squareX = centerX - squareSize / 2;
            int squareY = centerY - squareSize / 2;

            // Desenha o quadrado menor
            engine::draw::rect(squareX, squareY, squareSize, squareSize, blackColor);
        }

        // Paredes
        engine::draw::rect(xpos, ypos, m_cell_size, wall_thickness, blackColor);
        engine::draw::rect(xpos + m_cell_size - wall_thickness, ypos, wall_thickness, m_cell_size, blackColor);
        engine::draw::rect(xpos, ypos + m_cell_size - wall_thickness, m_cell_size, wall_thickness, blackColor);
        engine::draw::rect(xpos, ypos, wall_thickness, m_cell_size, blackColor);
    }

    void Cell::update(unsigned) {

    };

    void Cell::renderTexture(int xpos, int ypos, const std::string &filename) const
    {
        int wt = Cell::wall_thickness-1;
        auto cellTexture = engine::loadTexture(filename);
        if (!cellTexture) {
            return;
        }

        SDL_Rect destRect{xpos+wt, ypos+wt, (m_cell_size-2*wt), (m_cell_size-2*wt)};
        SDL_Point center{(m_cell_size-2*wt) / 2, (m_cell_size-2*wt) / 2};
        SDL_RenderCopyEx(engine::getRenderer(), cellTexture.get(), nullptr, &destRect, 0, &center, SDL_FLIP_NONE);
    }
}