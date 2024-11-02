#ifndef RC_ENGINE_H
#define RC_ENGINE_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <memory>

namespace engine {

    bool init();
    void close();
    SDL_Renderer* getRenderer();
    std::shared_ptr<SDL_Texture> loadTexture(const std::string&filename);
    void renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color);


    namespace draw {

        void rect(int x, int y, int w, int h, SDL_Color color);
        void clearRect(int x, int y, int w, int h);

    }

    namespace screen {

        void clear();
        void show();

    }
}

#endif
