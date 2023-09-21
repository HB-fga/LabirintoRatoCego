#ifndef RC_MAZE_H
#define RC_MAZE_H

#include "gameObject.h"
#include "cell.h"
#include "rat.h"
#include <vector>
#include <fstream>
#include <stdexcept>

namespace game {

    class Maze : public GameObject {
    public:
        Maze(int rows = 1, int cols = 1);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;
		void setcell(int xpos, int ypos, Cell cell);
        static Maze fromFile(const std::string& filename, const std::string& movementsFilename);

    private:
        int rows, cols;
        std::vector<std::vector<Cell>> maze;
        std::vector<char> movements;
        game::Rat rat;
    };
}

#endif
