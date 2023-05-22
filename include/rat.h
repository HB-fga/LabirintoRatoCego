#ifndef RC_RAT_H
#define RC_RAT_H

namespace game {

    class Rat {
    public:
        Rat(int rows, int cols, int cell_size, int xpos = 1, int ypos = 1);
        void draw() const;
        void update();


    private:
        int rows, cols, cell_size;
        int xpos, ypos;
    };
}

#endif
