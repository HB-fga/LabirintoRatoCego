#include "drawRanking.h"

#include <algorithm>

DrawRanking::DrawRanking(int screenWidth, int screenHeight)
    : SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight) {
}

DrawRanking::~DrawRanking() {
}

void DrawRanking::drawRanking(std::vector<std::tuple<std::string, std::string, std::string>> mouseData) const {
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
    engine::renderText("RANKING Labirinto do Rato Cego", SCREEN_WIDTH / 4, 20, font30p, whiteColor);

    std::sort(mouseData.begin(), mouseData.end(), [](const auto& a, const auto& b) {
        return std::stoi(std::get<1>(a)) < std::stoi(std::get<1>(b));
    });


    engine::renderText("Nome", 60, 100, font30p, whiteColor);
    engine::renderText("Qtd Movimentos", SCREEN_WIDTH / 5 + 400, 100, font30p, whiteColor);
    engine::renderText("Rato", 1740, 100, font30p, whiteColor);

    int y = 170;
    for (const auto& mouse : mouseData) {

        engine::renderText(std::get<0>(mouse), 60, y, font30p, blackColor);
        engine::renderText(std::get<1>(mouse), SCREEN_WIDTH / 5 + 550, y, font30p, blackColor);
        y += 70;
    }

    int x = 170;
    for (const auto& mouse : mouseData) {
        const int ratWidth{ 40 }, ratHeight{ 48 };

        auto ratTexture = engine::loadTexture(std::get<2>(mouse));
        if (!ratTexture) {
            return;
        }
        
        SDL_Rect destRect{ 1780, x, ratWidth, ratHeight };
        SDL_RenderCopy(engine::getRenderer(), ratTexture.get(), nullptr, &destRect);
        x += 60;
    }

    engine::renderText("Pressione ESC para fechar o jogo", 60, 1000, font30p, whiteColor);

    TTF_CloseFont(font30p);
}

