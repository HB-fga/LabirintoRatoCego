#ifndef RC_RAT_H
#define RC_RAT_H

#include "gameObject.h"
#include <utility>
#include <cstdint>
#include <vector>
#include <fstream>

namespace game {

    class Rat : public GameObject{
    public:
        Rat(int xv, int yv, const std::string& textureFilename);
        void update(unsigned ticks);
        void reverseUpdate(unsigned ticks);
        void setMovements(const std::vector<std::pair<int, int>>& movements);
        void draw(int xpos, int ypos) const;
        std::pair<int, int> getPos() const { return { x, y };}
        std::pair<int, int> from() const { return { x, y };}
        std::pair<int, int> to() const { return { dest_x, dest_y };}
        double getProgress() const{ return progress; }
        int getIndex() const;


    private:
        int x, y;
        uint32_t last_ticks;
        int current_movement_index;
        int direction;
        bool is_flip;
        std::vector<std::pair<int, int>> movements;
        std::string textureFilename;
        int dest_x, dest_y;
        bool moving;
        double progress;
        int frame;
    };
}

#endif
