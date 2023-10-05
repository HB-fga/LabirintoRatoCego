#include "maze.h"
#include "engine.h"
#include <fstream>
#include <iostream>

namespace game
{

    Maze::Maze(int r, int c)
        : rows(r), cols(c), maze(r, std::vector<Cell>(c)), rat(6, 7)
    {
    }

    void Maze::update(unsigned ticks)
    {
        rat.setMovements(movements);
        rat.update(ticks);
    }

    void Maze::setcell(int xpos, int ypos, Cell cell)
    {
        maze[xpos][ypos] = cell;
    }

    Maze Maze::fromFile(const std::string &filename, const std::string& movementsFilename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::invalid_argument("Failed to open file: " + filename);
        }
        int rows, cols, cell_size;
        file >> cols >> rows >> cell_size;
        Maze maze(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int cellValue;
                if (file >> cellValue)
                {
                    bool not_valid = false;
                    bool is_valid = false;
                    bool is_decision = false;
                    bool maze_exit = false;

                    if (cellValue == 0)
                        not_valid = true;
                    if (cellValue == 1)
                        is_valid = true;
                    if (cellValue == 2)
                        is_decision = true;
                    if (cellValue == 3)
                        maze_exit = true;

                    maze.setcell(i, j, Cell(not_valid, is_valid, is_decision, maze_exit, cell_size));
                }
                else
                {
                    file.close();
                    throw std::invalid_argument("Invalid cell value in file: " + filename);
                }
            }
        }

        std::ifstream movementsFile(movementsFilename);
        if (!movementsFile.is_open()) {
            throw std::invalid_argument("Failed to open movements file: " + movementsFilename);
        }

        std::vector<char> movements;
        char movement;
        while (movementsFile >> movement) {
            movements.push_back(movement);
        }

        maze.movements = movements;  // Assign movements to the maze

        // std::cout << "MOVEMENTS = " << movements.size() << "\n";

        file.close();
        movementsFile.close();
        return maze;
    }

    void Maze::draw(int xpos, int ypos) const
    {
        int cell_size = maze[0][0].cell_size();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                maze[i][j].draw(xpos + j * cell_size, ypos + i * cell_size);
            }
        }

        auto p = rat.getPos();
        // rat.draw(xpos + p.first * cell_size + Cell::wall_thickness, ypos + p.second * cell_size + Cell::wall_thickness);
    }

}
