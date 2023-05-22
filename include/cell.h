#ifndef RC_CELL_H
#define RC_CELL_H

#include "gameObject.h"

namespace game {

    class Cell : public GameObject {
    public:
        Cell(bool norte = false, bool leste = false, bool sul = false, bool oeste = false, bool saida = false, int cell_size = 20);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;
        int cell_size() const { return m_cell_size; };

    private:
        bool norte, leste, sul, oeste, saida;
        int m_cell_size;


    };
}


#endif