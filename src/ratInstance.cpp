#include "ratInstance.h"
#include "engine.h"

RatInstance::RatInstance(int xv, int yv, const std::string& movementFile, const std::string& mapFile)
    : rat(xv, yv), movements(LoadMovementsFromFile(movementFile))
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    std::ifstream mapF(mapFile);
    if (!mapF.is_open())
    {
        throw std::invalid_argument("Failed to open file: " + mapFile);
    }

    int rows, cols;
    mapF >> cols >> rows;

    int mazeWidth = cols * 60;
    int mazeHeight = rows * 60;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int cellValue;
            if (mapF >> cellValue)
            {
                if (cellValue == 4){
                    this->xpos = (j-1) * 60 + (screenWidth - mazeWidth) / 2;
                    this->ypos = i * 60 + (screenHeight - mazeHeight) / 2;
                }
            }
            else
            {
                mapF.close();
                throw std::invalid_argument("Invalid cell value in file: " + mapFile);
            }

        }
    }
    mapF.close();
}

void RatInstance::update(unsigned ticks)
{
    rat.setMovements(movements);
    rat.update(ticks);
}

void RatInstance::draw() const
{
    auto p = rat.getPos();
    rat.draw(xpos + p.first * 60 + 5, ypos + p.second * 60 + 5);
}

std::vector<char> RatInstance::LoadMovementsFromFile(const std::string& movementsFilename)
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