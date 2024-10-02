#include "gameDesign.h"
#include "engine.h"

GameDesign::GameDesign(int screenWidth, int screenHeight)
    : SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight) {

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

void GameDesign::drawButtonSpeedy(int speed) const {
    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 0 };
    SDL_Color grayDarkColor{ 64, 64, 64, 255 };

    TTF_Font* font18p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 18);
    if (font18p == nullptr) {
        return;
    }

    // - Velocidade
    int minusPos = 800; 
    engine::draw::rect(minusPos, 995, 40, 40, grayDarkColor);
    engine::draw::rect(minusPos+5, 1000, 30, 30, blackColor);
    engine::renderText("-", minusPos+11, 1005+2, font18p, whiteColor);

    // + Velocidade
    int plusPos = 1085;
    engine::draw::rect(plusPos, 995, 40, 40, grayDarkColor);
    engine::draw::rect(plusPos+5, 1000, 30, 30, blackColor);
    engine::renderText("+", plusPos+11, 1005+2, font18p, whiteColor);

    // Pause Temp
    int pausePos = 940;
    engine::draw::rect(pausePos, 1035, 40, 40, grayDarkColor);
    engine::draw::rect(pausePos+5, 1040, 30, 30, blackColor);
    engine::renderText("||", pausePos+2, 1046, font18p, whiteColor);

    // Medidor de velocidade + " célula" + (speed>1? "s" : "") + "/s"
    engine::renderText((speed ? "Velocidade: " + std::to_string(speed) : "   Pausado   "), 845, 1005+1, font18p, blackColor);
    TTF_CloseFont(font18p);
}

void GameDesign::drawButtonReverse(bool reverse) const {
    SDL_Color blackColor{ 0, 0, 0, 0 };
    SDL_Color redColor{255, 0, 0, 0};
    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color grayDarkColor{ 64, 64, 64, 255 };

    TTF_Font* font18p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 18);
    if (font18p == nullptr) {
        return;
    }

    int reversePos = 890;
    engine::draw::rect(reversePos, 1035, 40, 40, grayDarkColor);
    engine::draw::rect(reversePos+5, 1040, 30, 30, blackColor);
    engine::renderText("R", reversePos+12, 1046, font18p, (reverse ? redColor : whiteColor));

    TTF_CloseFont(font18p);
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
    engine::renderText("Pressione ESC para fechar o jogo", 1060, 1000, font25p, whiteColor);


    TTF_CloseFont(font15p);
    TTF_CloseFont(font25p);
}
