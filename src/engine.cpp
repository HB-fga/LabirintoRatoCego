#include "engine.h"

namespace engine {

    static SDL_Window* gWindow = nullptr;
    static SDL_Renderer* gRenderer = nullptr;
    static std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> gTextureCache;

    bool init()
    {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        auto w = DM.w;
        auto h = DM.h;

        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texture filtering not enabled!\n";
        }

        // Create window
        gWindow = SDL_CreateWindow("Labirinto do Rago Cego", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

        if (not gWindow)
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        // Create vsynced renderer for window
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (not gRenderer)
        {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        // Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;

        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            return false;
        }

        if( TTF_Init() == -1 )
        {
            printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
            return false;
        }

        SDL_RenderSetLogicalSize(gRenderer, 1920, 1080);

        return true;
    }

    void close()
    {
        // Destroy window
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
        gRenderer = nullptr;

        // Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Renderer* getRenderer()
    {
        return gRenderer;
    }

    std::shared_ptr<SDL_Texture> loadTexture(const std::string&filename)
    {
        // Check if the texture is already loaded
        auto it = gTextureCache.find(filename);
        if (it != gTextureCache.end()) {
            return it->second;
        }

        // Load the texture
        SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
        if (!loadedSurface) {
            std::cerr << "Failed to load texture image '" << filename << "'! SDL_image Error: " << IMG_GetError() << '\n';
            return nullptr;
        }

        // Create texture from surface
        SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (!texture) {
            std::cerr << "Failed to create texture from surface! SDL Error: " << SDL_GetError() << '\n';
            SDL_FreeSurface(loadedSurface);
            return nullptr;
        }

        // Store the texture in the cache
        std::shared_ptr<SDL_Texture> sharedTexture(texture, SDL_DestroyTexture);
        gTextureCache[filename] = sharedTexture;

        // Free the loaded surface
        SDL_FreeSurface(loadedSurface);

        return sharedTexture;
    }

    void renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color)
    {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
        if (surface == nullptr) {
            // Trate o erro ao renderizar o texto
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(engine::getRenderer(), surface);
        if (texture == nullptr) {
            // Trate o erro ao criar a textura
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect destRect{ x, y, surface->w, surface->h };
        SDL_RenderCopy(engine::getRenderer(), texture, nullptr, &destRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    namespace draw {

        void rect(int x, int y, int w, int h, SDL_Color color)
        {
            SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
            SDL_Rect rect{ x, y, w, h };
            SDL_RenderFillRect(gRenderer, &rect);
        }

        void clearRect(int x, int y, int w, int h)
        {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
        }

    }

}
