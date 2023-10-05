#ifndef RC_MAPRENDER_H
#define RC_MAPRENDER_H

#include <iostream>
#include <fstream>

class MapRenderer {
    public:
        MapRenderer(int screenWidth, int screenHeight, int mazeWidth, int mazeHeight);
        ~MapRenderer();

        void updateCellColor(int x, int y);
        void generateMaze();
        void saveMapToFile(const std::string& filePath);

        int buttonWidth;
        int buttonHeight;
        int buttonX;
        int buttonY;
        
    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        int MAZE_WIDTH;
        int MAZE_HEIGHT;
        int CELL_SIZE;

        enum CellType {
            CELL_EMPTY,
            CELL_DECISION,
            CELL_PATH,
            CELL_EXIT,
            CELL_FORBIDDEN
        };

        CellType** mapMaze;
};

#endif