#ifndef RC_MAZE_H
#define RC_MAZE_H

#include "gameObject.h"
#include "cell.h"
#include "rat.h"
#include "gameDesign.h"
#include <vector>
#include <fstream>
#include <stdexcept>

namespace game {

    class Maze : public GameObject {
    public:
        Maze(int rows = 1, int cols = 1);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;
        void drawSelection(int xpos, int ypos) const;
        void drawCentered(bool is_selection = false) const;
		void setcell(int xpos, int ypos, Cell cell);
        static Maze loadMapfromFile(const std::string& filename);
        std::vector<char> loadMovementsFromFile(const std::string& movementsFilename);
        void setMovements(const std::vector<char> &movements);

    private:
        int rows, cols;
        std::vector<std::vector<Cell>> maze;
        std::vector<char> movements;
        game::Rat rat;
        GameDesign gameDesign;

    };
}

#endif
