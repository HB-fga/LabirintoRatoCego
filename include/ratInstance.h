#ifndef RC_RATINSTANCE_H
#define RC_RATINSTANCE_H

#include "rat.h"
#include <vector>
#include <fstream>
#include <stdexcept>


class RatInstance {
public:
    RatInstance(int xv, int yv, const std::string& movementFile, const std::string& mapFile);
    void update(unsigned ticks);
    void draw() const;

    int xpos, ypos;

private:
    static std::vector<char> LoadMovementsFromFile(const std::string& movementsFilename);
    int x, y;
    game::Rat rat;
    std::vector<char> movements;
};


#endif