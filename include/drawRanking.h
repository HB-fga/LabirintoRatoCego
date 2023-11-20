#ifndef RC_DRAWRANKING_H
#define RC_DRAWRANKING_H

#include "engine.h"
#include <vector>
#include <string>

class DrawRanking {

    public:
        DrawRanking(int screenWidth, int screenHeight);
        ~DrawRanking();

        // drawRanking() receives a vector of tuples containing the mouse name ,the number of movements and image path rat
        void drawRanking(std::vector<std::tuple<std::string, std::string, std::string>> mouseData) const;

    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
};

#endif