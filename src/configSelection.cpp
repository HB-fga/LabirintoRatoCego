#include "configSelection.h"
#include "engine.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace game
{
    ConfigSelection::ConfigSelection(const std::string &mapsDirectory)
    {
        // Percorre os arquivos no diretório de mapas e os adiciona à lista de mapFiles
        for (const auto &entry : fs::directory_iterator(mapsDirectory))
        {
            if (entry.is_regular_file())
            {
                mapFiles.push_back(entry.path().string());
            }
        }

        // Inicializa o índice selecionado para o primeiro mapa
        selectedMapIndex = 0;
    }

    std::string ConfigSelection::getSelectedMap() const
    {
        // Retorna o caminho completo do mapa selecionado
        return mapFiles[selectedMapIndex];
    }

    void ConfigSelection::navigateUp()
    {
        // Navega para cima na lista de mapas
        if (selectedMapIndex > 0)
        {
            selectedMapIndex--;
        }
    }

    void ConfigSelection::navigateDown()
    {
        // Navega para baixo na lista de mapas
        if (selectedMapIndex < static_cast<int>(mapFiles.size()) - 1)
        {
            selectedMapIndex++;
        }
    }

    void ConfigSelection::writeTextSelection(const std::string &mapsDirectory, int height)
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


        size_t lastSlashPos = mapsDirectory.find_last_of("/\\");
        std::string mapFileName = mapsDirectory.substr(lastSlashPos + 1);
        size_t extensionPos = mapFileName.find(".txt");
        mapFileName = mapFileName.substr(0, extensionPos);

        engine::renderText(mapFileName, 1920 / 2 - 100, height, font25p, blueColor);
        
        if (height > 63){
            engine::renderText("Selecione os arquivos de movimento com as setas (Cima/Baixo)", 60, 63*3, font15p, whiteColor);
            return;
        }
        TTF_CloseFont(font25p);
        TTF_CloseFont(font15p);
    } 
}
