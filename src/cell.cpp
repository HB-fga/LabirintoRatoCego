#include "cell.h"
#include "engine.h"
#include <iostream>

namespace game {

    const int Cell::wall_thickness = 5;

    Cell::Cell(bool v, bool nv, bool n, bool l, bool s, bool o, bool maze_exit, int cell_size)
        : not_valid(v), is_valid(nv), north(n), east(l), south(s), west(o), maze_exit(maze_exit), m_cell_size(cell_size) {}

    void Cell::draw(int xpos, int ypos) const {
        SDL_Color greenColor { 0, 255, 0, 255 };
        SDL_Color blackColor { 0, 0, 0, 255 };
        SDL_Color grayDarkColor{ 64, 64, 64, 255 };
        SDL_Color grayLightColor{ 192, 192, 192, 255 };
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
        else {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, grayLightColor);

            // Calcule o centro do quadrado
            int centerX = xpos + m_cell_size / 2;
            int centerY = ypos + m_cell_size / 2;

            // Largura dos retângulos que compõem as setas
            int arrowWidth = m_cell_size / 6;

            int arrowSize = m_cell_size / 4;

            if (north) {
                // Desenhe a seta para o norte (uma seta apontando para cima)
                engine::draw::rect(centerX - arrowWidth / 2, centerY - (m_cell_size / 2), arrowWidth, m_cell_size / 2, blackColor);
                engine::draw::rect(centerX - arrowSize, centerY - (m_cell_size / 2), arrowSize, arrowWidth, blackColor);
                engine::draw::rect(centerX, centerY - (m_cell_size / 2), arrowSize, arrowWidth, blackColor);
            }

            if (east) {
                // Desenhe a seta para o leste (uma seta apontando para a direita)
                engine::draw::rect(centerX, centerY - arrowWidth / 2, m_cell_size / 2, arrowWidth, blackColor);
                engine::draw::rect(centerX + (m_cell_size / 2 - arrowWidth), centerY - arrowSize, arrowWidth, arrowSize, blackColor);
                engine::draw::rect(centerX + (m_cell_size / 2 - arrowWidth), centerY, arrowWidth, arrowSize, blackColor);
            }

            if (south) {
                // Desenhe a seta para o sul (uma seta apontando para baixo)
                engine::draw::rect(centerX - arrowWidth / 2, centerY, arrowWidth, m_cell_size / 2, blackColor);
                engine::draw::rect(centerX - arrowSize, centerY + (m_cell_size / 2 - arrowWidth), arrowSize, arrowWidth, blackColor);
                engine::draw::rect(centerX, centerY + (m_cell_size / 2 - arrowWidth), arrowSize, arrowWidth, blackColor);
            }

            if (west) {
                // Desenhe a seta para o oeste (uma seta apontando para a esquerda)
                engine::draw::rect(centerX - (m_cell_size / 2), centerY - arrowWidth / 2, m_cell_size / 2, arrowWidth, blackColor);
                engine::draw::rect(centerX - (m_cell_size / 2), centerY - arrowSize, arrowWidth, arrowSize, blackColor);
                engine::draw::rect(centerX - (m_cell_size / 2), centerY, arrowWidth, arrowSize, blackColor);
            }
        }
    }

    void Cell::update(unsigned) {

    };
}