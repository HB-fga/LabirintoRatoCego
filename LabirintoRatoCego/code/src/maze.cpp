#include "maze.h"
#include "engine.h"

namespace game {

    Maze::Maze(int r, int c, int s, int x, int y)
        : rows(r), cols(c), cell_size(s), xpos(x), ypos(y)
    {
    } 

    void Maze::draw() const
    {
        const int thickness = 5;

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                int x = xpos + cell_size * c;
                int y = ypos + cell_size * r;

                engine::draw::rect(x, y, cell_size, thickness);
            }
    }

}
