#include "maze.h"
#include "engine.h"
#include <fstream>
#include <iostream>

namespace game {

    Maze::Maze(int r, int c)
        : rows(r), cols(c), maze(r, std::vector<Cell>(c))
    {
        
    }

    void Maze::update(unsigned)
    {
    }

    bool Maze::loadMazeFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Failed to open file: " << filename << std::endl;
            return false;
        }
        for (int i = 0; i < rows; i++) {
           for (int j = 0; j < cols; j++) {
               int cellValue;
               if (file >> cellValue) {
                   std::cout << cellValue << std::endl;
                   bool norte = cellValue & 1;
                   bool leste = cellValue & 2;
                   bool sul = cellValue & 4;
                   bool oeste = cellValue & 8;
                   bool saida = cellValue & 16;
                   maze[i][j] = Cell(norte, leste, sul, oeste, saida, 50);
               } else {
                   std::cout << "Invalid cell value in file: " << filename << std::endl;
                   file.close();
                   return false;
               }
           }
        }

        file.close();
        return true;
    }

    void Maze::draw(int xpos, int ypos) const
    {
        int cell_size = maze[0][0].cell_size();
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                maze[i][j].draw(xpos + j * cell_size, ypos + i * cell_size);
            }
        }
    }

}
