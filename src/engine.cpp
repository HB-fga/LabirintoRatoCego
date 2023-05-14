#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "engine.h"

namespace engine {

    static SDL_Window* gWindow = nullptr;
    static SDL_Renderer* gRenderer = nullptr;

    bool init(int w, int h)
    {
        //I nitialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texture filtering not enabled!\n";
        }

        //Create window
        gWindow = SDL_CreateWindow("Labirinto do Rago Cego", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

        if (not gWindow)
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;

        }

        //Create vsynced renderer for window
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (not gRenderer)
        {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        //Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;

        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            return false;
        }

        return true;
    }

    void close()
    {
        //Destroy window
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
        gRenderer = nullptr;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    namespace draw {

        void rect(int x, int y, int w, int h)
        {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
            SDL_Rect rect{ x, y, w, h };
            SDL_RenderFillRect(gRenderer, &rect);
        }
    }

    namespace screen {

        void clear()
        {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
        }

        void show()
        {
            SDL_RenderPresent(gRenderer);
            SDL_Delay(10000);
        }

    }

}
