#include "maze.h"
#include "engine.h"
#include <fstream>
#include <iostream>

namespace game
{

    Maze::Maze(int r, int c)
        : rows(r), cols(c), maze(r, std::vector<Cell>(c)), rat(0, 0), gameDesign(1920, 1080)
    {
    }

    void Maze::update(unsigned ticks)
    {
        // rat.setMovements(movements);
        // rat.update(ticks);
    }

    void Maze::setcell(int xpos, int ypos, Cell cell)
    {
        maze[xpos][ypos] = cell;
    }

    Maze Maze::loadMapfromFile(const std::string &filename)
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
                    bool maze_start = false;

                    if (cellValue == 0)
                        not_valid = true;
                    if (cellValue == 1)
                        is_valid = true;
                    if (cellValue == 2)
                        is_decision = true;
                    if (cellValue == 3)
                        maze_exit = true;
                    if (cellValue == 4){
                        maze_start = true;
                        maze.rat = Rat(j, i);
                    }

                    maze.setcell(i, j, Cell(not_valid, is_valid, is_decision, maze_exit, maze_start, cell_size));
                }
                else
                {
                    file.close();
                    throw std::invalid_argument("Invalid cell value in file: " + filename);
                }
            }
        }

        file.close();
        return maze;
    }

    std::vector<char> Maze::loadMovementsFromFile(const std::string& movementsFilename)
    {
        std::ifstream movementsFile(movementsFilename);
        if (!movementsFile.is_open())
        {
            throw std::invalid_argument("Failed to open movements file: " + movementsFilename);
        }

        std::vector<char> movements;
        char movement;
        while (movementsFile >> movement)
        {
            movements.push_back(movement);
        }

        movementsFile.close();
        return movements;
    }

    // void Maze::setMovements(const std::vector<char>& movements)
    // {
    //     // this->movements = movements;
    //     // rat.setMovements(movements); // Atualizar os movimentos da rat
    // }

    void Maze::drawCentered(bool is_selection) const
    {
        int screenWidth = 1920;
        int screenHeight = 1080;
        int mazeWidth = cols * maze[0][0].cell_size();
        int mazeHeight = rows * maze[0][0].cell_size();

        int xpos = (screenWidth - mazeWidth) / 2;
        int ypos = (screenHeight - mazeHeight) / 2;

        if(!is_selection)
            draw(xpos, ypos);
        else
            drawSelection(xpos, ypos);
    }

    void Maze::draw(int xpos, int ypos) const
    {

        gameDesign.draw();

        int cell_size = maze[0][0].cell_size();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                maze[i][j].draw(xpos + j * cell_size, ypos + i * cell_size);
            }
        }

        // auto p = rat.getPos();
        // rat.draw(xpos + p.first * cell_size + Cell::wall_thickness, ypos + p.second * cell_size + Cell::wall_thickness);
    }

    void Maze::drawSelection(int xpos, int ypos) const
    {
        // engine::draw::rect(0, 0, 1920, 1080, { 192, 192, 192, 255 });
        gameDesign.drawSelection();

        int cell_size = maze[0][0].cell_size();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                maze[i][j].draw(xpos + j * cell_size, ypos + i * cell_size);
            }
        }
    }

}
