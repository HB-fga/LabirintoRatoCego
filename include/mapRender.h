#ifndef RC_MAPRENDER_H
#define RC_MAPRENDER_H

#include <iostream>
#include <fstream>
#include <string>

class MapRenderer {
    public:
        MapRenderer(int screenWidth, int screenHeight, int initialMazeWidth, int initialMazeHeight);
        ~MapRenderer();
        void increaseMazeWidth();
        void decreaseMazeWidth();
        void increaseMazeHeight();
        void decreaseMazeHeight();

        void updateCellColor(int x, int y);
        void generateMaze();
        void resetMap();
        void loadMapfromFile(const std::string& filePath);
        void saveMapToFile(const std::string& filePath);

        int buttonWidth;
        int buttonHeight;
        int buttonXgererated;
        int buttonYgererated;
        int buttonXreset;
        int buttonYreset;
        
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
            CELL_FORBIDDEN,
            CELL_START
        };

        CellType** mapMaze;
};

#endif