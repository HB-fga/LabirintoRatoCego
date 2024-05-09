#include "maze.h"
#include "engine.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using pJSON = nlohmann::json;

namespace game
{

    Maze::Maze(int r, int c)
        : rows(r), cols(c), maze(r, std::vector<Cell>(c)), gameDesign(1920, 1080)
    {
    }

    void Maze::update(unsigned ticks)
    {
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

        pJSON jsonFile = pJSON::parse(file);

        int rows = jsonFile["height"], cols = jsonFile["width"], cell_size = jsonFile["cellSize"];
        Maze maze(rows, cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int cellValue = jsonFile["map"][i][j];
                if (cellValue >= 0 && cellValue < 5) 
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
    }

    void Maze::drawSelection(int xpos, int ypos) const
    {
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

    int Maze::validatorMovement(const std::string& movementsFilename, const std::string& mapFilename)
    {
        std::ifstream movementsFile(movementsFilename);
        if (!movementsFile)
        {
            // TODO: Tratar exceção ou abortar o código
            throw std::invalid_argument("Failed to open movements file: " + movementsFilename);
        }

        pJSON jsonMoves = pJSON::parse(movementsFile);

        // TODO: Tem no ratInstance, verificar se pode ser retirado de lá
        auto path = jsonMoves["path"];
        std::vector<std::pair<int, int>> movements;
        for (auto& move : path)
            movements.push_back({move["row"], move["col"]});
        
        std::ifstream mapF(mapFilename);
        if (!mapF)
        {
            // TODO: Tratar exceção ou abortar o código
            throw std::invalid_argument("Failed to open file: " + mapFilename);
        }

        pJSON jsonFile = pJSON::parse(mapF);
        int rows = jsonFile["height"], cols = jsonFile["width"];
        
        auto mazeV = jsonFile["map"];
        for(auto& row : mazeV)
        {
            auto [a, b] = minmax_element(row.begin(), row.end());
            if (*a < 0 or *b > 4) 
            {
                // TODO: Tratar exceção ou abortar o código
                throw std::invalid_argument("Invalid cell value in file: " + mapFilename);
            }
        }

        int rowExit = jsonFile["exit"]["row"], colExit = jsonFile["exit"]["col"];

        for (const auto& [row, col] : movements)
        {
            // Verificar se as coordenadas estão dentro dos limites do labirinto
            if (row < 0 || row >= rows || col < 0 || col >= cols)
            {
                return -1;
            }

            // Verificar se o valor correspondente no labirinto é diferente de zero
            if (mazeV[row][col] == 0)
            {
                return -1;
            }
        }

        // verificar se o rato chegou à saída
        if ((movements.back().first != colExit || movements.back().second != rowExit) && movements.size() < 999)
        {
            return 1;
        }
        
        movementsFile.close();
        mapF.close();

        return 0;
    }

}
