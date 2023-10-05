#ifndef RC_CELL_H
#define RC_CELL_H

#include "gameObject.h"

namespace game {

    class Cell : public GameObject {
    public:
        Cell(bool not_valid = false, bool is_valid = false, bool is_decision = false, bool maze_exit = false, int cell_size = 20);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;
        int cell_size() const { return m_cell_size; };
        static const int wall_thickness;

    private:
        bool not_valid, is_valid, is_decision, maze_exit;
        int m_cell_size;
    };
}


#endif