#ifndef RC_GAMEDESIGN_H
#define RC_GAMEDESIGN_H

class GameDesign {
    public:
        GameDesign(int screenWidth, int screenHeight);
        void draw() const;
        
    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
};

#endif