#ifndef RC_RAT_H
#define RC_RAT_H

#include "gameObject.h"

namespace game {

    class Rat : public GameObject{
    public:
        Rat(int xv, int yv);
        void update(unsigned ticks);
        void draw(int xpos, int ypos) const;


    private:
        int x, y;
    };
}

#endif
