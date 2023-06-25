#include "cell.h"
#include "engine.h"
#include <iostream>

namespace game {

    const int Cell::wall_thickness = 5;

    Cell::Cell(bool n, bool l, bool s, bool o, bool mazeExit, int cell_size)
        : north(n), east(l), south(s), west(o), mazeExit(mazeExit), m_cell_size(cell_size) {}

    void Cell::draw(int xpos, int ypos) const {
        SDL_Color greenColor { 0, 255, 0, 255 };
        SDL_Color blackColor { 0, 0, 0, 255 };
        int wt = Cell::wall_thickness - 3;

        if (mazeExit) {
            engine::draw::rect(xpos, ypos, m_cell_size, m_cell_size, greenColor);
        }
        if (north) {
            engine::draw::rect(xpos, ypos, m_cell_size, wt, blackColor);
        }
        if (east) {
            engine::draw::rect(xpos + m_cell_size - wt, ypos, wt, m_cell_size, blackColor);
        }
        if (south) {
            engine::draw::rect(xpos, ypos + m_cell_size - wt, m_cell_size, wt, blackColor);
        }
        if (west) {
            engine::draw::rect(xpos, ypos, wt, m_cell_size, blackColor);
        }
    }

    void Cell::update(unsigned) {

    };
}