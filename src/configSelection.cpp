#include "configSelection.h"
#include <filesystem>

using pJSON = nlohmann::json;
namespace fs = std::filesystem;

// TODO: Essa classe não é utilizada apenas para mapas, renomear de acordo
namespace game
{
    ConfigSelection::ConfigSelection(const std::string &mapsDirectory, std::string filter)
    {
        font25p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 25);
        if (font25p == nullptr) {
            return;
        }

        font15p = TTF_OpenFont("./assets/fonts/PressStart2P-Regular.ttf", 15);
        if (font15p == nullptr) {
            return;
        }

        // Percorre os arquivos no diretório de mapas e os adiciona à lista de mapFiles
        for (const auto &entry : fs::directory_iterator(mapsDirectory))
        {
            if (entry.is_regular_file())
            {
                if(filter != "")
                {
                    std::ifstream inputFile(entry.path().string());
                    pJSON movementFile = pJSON::parse(inputFile);
                    std::string mapName = movementFile["mapName"];
                    inputFile.close();

                    if(mapName == filter)
                    {
                        mapFiles.push_back(entry.path().string());
                    }
                }
                else
                {
                    mapFiles.push_back(entry.path().string());
                }
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

    std::string ConfigSelection::getSelectedMapPretty() const
    {
        // Retorna o caminho completo do mapa selecionado
        std::string fileName = mapFiles[selectedMapIndex];
        size_t lastSlashPos = fileName.find_last_of("/\\");
        fileName = fileName.substr(lastSlashPos + 1);
        return fileName;
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

        size_t lastSlashPos = mapsDirectory.find_last_of("/\\");
        std::string mapFileName = mapsDirectory.substr(lastSlashPos + 1);
        size_t extensionPos = mapFileName.find(".json");
        mapFileName = mapFileName.substr(0, extensionPos);

        engine::renderText(mapFileName, 1920 / 2 - 100, height, font25p, blueColor);
        
        if (height > 63){
            engine::renderText("Selecione os arquivos de movimento com as setas (Cima/Baixo)", 60, 63*3, font15p, whiteColor);
            return;
        }
    } 
}
