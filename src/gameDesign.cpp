#include "gameDesign.h"
#include "engine.h"

GameDesign::GameDesign(int screenWidth, int screenHeight)
    : SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight) {

    // buttonWidth = 200;
    // buttonHeight = 30;
    // buttonXgererated = (SCREEN_WIDTH - buttonWidth) / 2;
    // buttonYgererated = SCREEN_HEIGHT - 80;

    // buttonXreset = (SCREEN_WIDTH - MAZE_WIDTH * CELL_SIZE) / 2;
    // buttonYreset = SCREEN_HEIGHT - 80;

    // mapMaze = new CellType*[MAZE_HEIGHT];
    // for (int i = 0; i < MAZE_HEIGHT; ++i) {
    //     mapMaze[i] = new CellType[MAZE_WIDTH]();
    // }
}

void GameDesign::draw() const{
    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 0 };
    SDL_Color grayDarkColor{ 64, 64, 64, 255 };
    SDL_Color grayLightColor{ 192, 192, 192, 255 };

    engine::draw::rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, grayLightColor);
    engine::draw::rect(0, 0, SCREEN_WIDTH, 60, grayDarkColor);
    engine::draw::rect(10, 10, SCREEN_WIDTH - 20, 40, blackColor);


    TTF_Font* font30p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 30);
    if (font30p == nullptr) {
        return;
    }
    engine::renderText("Labirinto do Rato Cego", SCREEN_WIDTH / 3, 20, font30p, whiteColor);
    TTF_CloseFont(font30p);
}

void GameDesign::drawSelection() const{
    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blueColor{ 0, 0, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 0 };
    SDL_Color grayDarkColor{ 64, 64, 64, 255 };
    SDL_Color grayLightColor{ 192, 192, 192, 255 };

    engine::draw::rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, grayLightColor);
    engine::draw::rect(0, 0, 1920, 60, grayDarkColor);
    engine::draw::rect(10, 10, 1920 - 20, 40, blackColor);

    TTF_Font* font30p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 30);
    if (font30p == nullptr) {
        return;
    }
    engine::renderText("Selecione o Mapa", 1920 / 4 + 195, 20, font30p, whiteColor);
    TTF_CloseFont(font30p);

    TTF_Font* font15p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 15);
    if (font15p == nullptr) {
        return;
    }

    TTF_Font* font25p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 25);
    if (font15p == nullptr) {
        return;
    }

    engine::renderText("Selecione o labirinto com as setas (Cima/Baixo)", 60, 70, font15p, whiteColor);
    engine::renderText("Pressione ENTER para confirmar", 60, 1000, font25p, whiteColor);
    TTF_CloseFont(font15p);
    TTF_CloseFont(font25p);
}
