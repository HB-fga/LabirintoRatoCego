#include "ratSelection.h"
#include "engine.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace game {

    RatSelection::RatSelection()
    : selectedQuantity(1), minQuantity(1), maxQuantity(6)
    {

    }

    int RatSelection::getSelectedQuantity() const
    {
        return selectedQuantity;
    }

    void RatSelection::navigateRight()
    {
        selectedQuantity = std::min(selectedQuantity + 1, maxQuantity);
    }

    void RatSelection::navigateLeft()
    {
        selectedQuantity = std::max(selectedQuantity - 1, minQuantity);
    }

    void RatSelection::writeQuantitySelection() const
    {
        SDL_Color blueColor{ 0, 0, 255, 255 };
        SDL_Color whiteColor{ 255, 255, 255, 255 };

        TTF_Font* font25p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 25);
        if (font25p == nullptr) {
            return;
        }
        TTF_Font* font15p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 15);
        if (font15p == nullptr) {
            return;
        }

        std::string quantity = std::to_string(selectedQuantity);


        engine::renderText("Selecione a quantidade de ratos com as setas (Direita/Esquerda)", 60, 70, font15p, whiteColor);
        std::string text =  "Ratos: " + quantity;
        engine::renderText(text, 1920 / 2 - 60, 63*2, font25p, blueColor);
        engine::renderText("Pressione ENTER para confirmar", 60, 1000, font25p, whiteColor);


        TTF_CloseFont(font25p);
        TTF_CloseFont(font15p);
    }

}