#ifndef RC_MAZE_H
#define RC_MAZE_H

namespace game {

    class Maze {
    public:
        Maze(int rows, int cols, int cell_size, int xpos = 0, int ypos = 0);
    
        void draw() const; 

    private:
        int rows, cols, cell_size;
        int xpos, ypos;
    };
}

#endif
