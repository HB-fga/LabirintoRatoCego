#include "ratInstance.h"
#include "engine.h"
#include "../libs/json.hpp"

using pJSON = nlohmann::json;

RatInstance::RatInstance(int xv, int yv, const std::string& imageRat, const std::string& movementFile, const std::string& mapFile)
    : rat(xv, yv, imageRat), movements(loadMovementsFromFile(movementFile))
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    std::ifstream mapF(mapFile);
    if (!mapF.is_open())
    {
        throw std::invalid_argument("Failed to open file: " + mapFile);
    }

    pJSON jsonFile = pJSON::parse(mapF);
    int rows = jsonFile["height"], cols = jsonFile["width"];

    int mazeWidth = cols * 60;
    int mazeHeight = rows * 60;

    this->xpos = (screenWidth - mazeWidth) / 2;
    this->ypos = (screenHeight - mazeHeight) / 2;

    mapF.close();
}

void RatInstance::update(unsigned ticks)
{
    rat.setMovements(movements);
    rat.update(ticks);
}

void RatInstance::reverseUpdate(unsigned ticks)
{
    rat.setMovements(movements);
    rat.reverseUpdate(ticks);
}

void RatInstance::draw() const
{
    auto p = rat.getPos();
    rat.draw(xpos + p.first * 60 + 5, ypos + p.second * 60 + 5);
}

int RatInstance::getIndex() const
{
    return rat.getIndex();
}

std::vector<std::pair<int, int>> RatInstance::loadMovementsFromFile(const std::string& movementsFilename)
{
    std::ifstream movementsFile(movementsFilename);
    if (!movementsFile.is_open())
    {
        throw std::invalid_argument("Failed to open movements file: " + movementsFilename);
    }
    pJSON jsonFile = pJSON::parse(movementsFile);
    movementsFile.close();

   auto path = jsonFile["path"];
    std::vector<std::pair<int, int>> movements;
    for (auto& move : path)
        movements.push_back({move["col"], move["row"]});

    return movements;
}