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
        int wt = Cell::wall_thickness - 3;

        if (not_valid){
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, grayDarkColor);
        } 
        else if (is_valid) {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, grayLightColor);
        }
        else if (maze_exit) {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, greenColor);
        } 
        else if (maze_start) {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, blueColor);
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
}