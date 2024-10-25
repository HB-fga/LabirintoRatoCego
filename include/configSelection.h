#ifndef RC_CONFIGSELECTION_H
#define RC_CONFIGSELECTION_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "engine.h"

namespace game
{
    class ConfigSelection
    {
    private:
        std::vector<std::string> mapFiles;  // Lista de nomes de arquivo de mapas
        int selectedMapIndex;               // Índice do mapa selecionado
        TTF_Font* font25p;
        TTF_Font* font15p;

    public:
        ConfigSelection(const std::string &filesDirectory, std::string hash = "");
        std::string getSelectedMap() const;
        std::string getSelectedMapPretty() const;
        void navigateUp();
        void navigateDown();
        void writeTextSelection(const std::string &mapsDirectory, int height = 63);
        int getNumberOfFiles();
        void writeErrorMsg(const std::string &msg);
    };
}

#endif
