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
    auto [from_x, from_y] = rat.from();
    auto [to_x, to_y] = rat.to();
    auto ux = to_x-from_x, uy = to_y-from_y;
    auto p = std::min(rat.getProgress(), 1.0);

    rat.draw(xpos + (from_x + p * ux) * 60, ypos + (from_y + p * uy) * 60);
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