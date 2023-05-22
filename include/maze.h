#ifndef RC_MAZE_H
#define RC_MAZE_H

#include "gameObject.h"
#include "cell.h"
#include <vector>
#include <fstream>

namespace game {

    class Maze : public GameObject {
    public:
        Maze(int rows, int cols);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;
        bool loadMazeFromFile(const std::string& filename);

    private:
        int rows, cols;
        std::vector<std::vector<Cell>> maze; // matriz para representar o labirinto
    };
}

#endif
