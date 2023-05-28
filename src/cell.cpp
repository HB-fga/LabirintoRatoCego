#include "cell.h"
#include "engine.h"
#include <iostream>

namespace game {

    Cell::Cell(bool n, bool l, bool s, bool o, bool saida, int cell_size)
        : norte(n), leste(l), sul(s), oeste(o), saida(saida), m_cell_size(cell_size) {}

    void Cell::draw(int xpos, int ypos) const {
        const int wall_thickness { 2 };
        SDL_Color greenColor { 0, 255, 0, 255 };
        SDL_Color blackColor { 0, 0, 0, 255 };

        if (saida) {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, greenColor);
        }
        if (norte) {
            engine::draw::rect(xpos, ypos, m_cell_size, wall_thickness, blackColor);
        }
        if (leste) {
            engine::draw::rect(xpos + m_cell_size - wall_thickness, ypos, wall_thickness, m_cell_size, blackColor);
        }
        if (sul) {
            engine::draw::rect(xpos, ypos + m_cell_size - wall_thickness, m_cell_size, wall_thickness, blackColor);
        }
        if (oeste) {
            engine::draw::rect(xpos, ypos, wall_thickness, m_cell_size, blackColor);
        }
    }

    void Cell::update(unsigned) {

    };
}