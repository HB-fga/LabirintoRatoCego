#include "rat.h"
#include "maze.h"
#include "engine.h"

namespace game {

    Rat::Rat(int r, int c, int s, int x, int y)
        : rows(r), cols(c), cell_size(s), xpos(x), ypos(y) {
        xpos *= 10;
        ypos *= 10;
    }

    void Rat::draw() const {
        const int rat_size = 10;
        SDL_Color blackColor = { 0, 0, 0, 255 };

        int x = xpos;
        int y = ypos;
        engine::draw::rect(x, y, cell_size, rat_size, blackColor);
    }

    void Rat::update() {
        const int rat_size = 10;
        engine::draw::clearRect(xpos, ypos, cell_size, rat_size);
        xpos = 10;
        ypos = 20;
    }

}