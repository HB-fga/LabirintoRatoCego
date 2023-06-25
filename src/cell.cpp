#include "cell.h"
#include "engine.h"
#include <iostream>

namespace game {

    const int Cell::wall_thickness = 5;

    Cell::Cell(bool n, bool l, bool s, bool o, bool saida, int cell_size)
        : norte(n), leste(l), sul(s), oeste(o), saida(saida), m_cell_size(cell_size) {}

    void Cell::draw(int xpos, int ypos) const {
        SDL_Color greenColor { 0, 255, 0, 255 };
        SDL_Color blackColor { 0, 0, 0, 255 };
        int wt = Cell::wall_thickness - 3;

        if (saida) {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, greenColor);
        }
        if (norte) {
            engine::draw::rect(xpos, ypos, m_cell_size, wt, blackColor);
        }
        if (leste) {
            engine::draw::rect(xpos + m_cell_size - wt, ypos, wt, m_cell_size, blackColor);
        }
        if (sul) {
            engine::draw::rect(xpos, ypos + m_cell_size - wt, m_cell_size, wt, blackColor);
        }
        if (oeste) {
            engine::draw::rect(xpos, ypos, wt, m_cell_size, blackColor);
        }
    }

    void Cell::update(unsigned) {

    };
}