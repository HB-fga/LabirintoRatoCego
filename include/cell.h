#ifndef RC_CELL_H
#define RC_CELL_H

#include "gameObject.h"

namespace game {

    class Cell : public GameObject {
    public:
        Cell(bool north = false, bool east = false, bool south = false, bool west = false, bool mazeExit = false, int cell_size = 20);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;
        int cell_size() const { return m_cell_size; };
        static const int wall_thickness;

    private:
        bool north, east, south, west, mazeExit;
        int m_cell_size;


    };
}


#endif