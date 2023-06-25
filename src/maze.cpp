#include "maze.h"
#include "engine.h"
#include <fstream>
#include <iostream>

namespace game {

    Maze::Maze(int r, int c)
        : rows(r), cols(c), maze(r, std::vector<Cell>(c)), rat(0, 0)
    {
        
    }

    void Maze::update(unsigned ticks)
    {
        rat.update(ticks);
    }

    void Maze::setcell(int xpos, int ypos, Cell cell){
        maze[xpos][ypos] = cell;
    }

    Maze Maze::fromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::invalid_argument("Failed to open file: " + filename);
        }
        int rows, cols, cell_size;
        file >> rows >> cols >> cell_size;
        Maze maze(rows, cols);
        for (int i = 0; i < rows; i++) {
           for (int j = 0; j < cols; j++) {
               int cellValue;
               if (file >> cellValue) {
                   bool norte = cellValue & 1;
                   bool leste = cellValue & 2;
                   bool sul = cellValue & 4;
                   bool oeste = cellValue & 8;
                   bool saida = cellValue & 16;
                   maze.setcell(i, j, Cell(norte, leste, sul, oeste, saida, cell_size));
               } else {
                   file.close();
                   throw std::invalid_argument("Invalid cell value in file: " + filename);
               }
           }
        }

        file.close();
        return maze;
    }

    void Maze::draw(int xpos, int ypos) const
    {
        int cell_size = maze[0][0].cell_size();
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                maze[i][j].draw(xpos + j * cell_size, ypos + i * cell_size);
            }
        }

        auto p = rat.getPos();
        rat.draw(xpos + p.first * cell_size + Cell::wall_thickness, ypos + p.second * cell_size + Cell::wall_thickness);
    }

}
