#ifndef RC_CONFIGSELECTION_H
#define RC_CONFIGSELECTION_H

#include <string>
#include <vector>

namespace game
{
    class ConfigSelection
    {
    private:
        std::vector<std::string> mapFiles;  // Lista de nomes de arquivo de mapas
        int selectedMapIndex;               // Índice do mapa selecionado

    public:
        ConfigSelection(const std::string &mapsDirectory);
        std::string getSelectedMap() const;
        void navigateUp();
        void navigateDown();
        void writeTextSelection(const std::string &mapsDirectory, int height = 63);
    };
}

#endif
