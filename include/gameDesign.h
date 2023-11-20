#ifndef RC_GAMEDESIGN_H
#define RC_GAMEDESIGN_H

#include "engine.h"
class GameDesign {
    public:
        GameDesign(int screenWidth, int screenHeight);
        void draw() const;
        void drawSelection() const;
        void drawButtonSpeedy() const;
        
    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
};

#endif