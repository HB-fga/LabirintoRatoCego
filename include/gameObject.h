#ifndef RC_GAMEOBJECT_H
#define RC_GAMEOBJECT_H

namespace game {

    class GameObject {
    public:
        virtual void update(unsigned ticks) = 0;
        virtual void draw(int xpos, int ypos) const = 0;

    };
}

#endif
