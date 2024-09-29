#include "engine.h"
#include "ratInstance.h"
#include "maze.h"
#include "rat.h"
#include "gameDesign.h"
#include "configSelection.h"
#include "ratSelection.h"
#include "drawRanking.h"
#include <nlohmann/json.hpp>

#include <iostream>

using pJSON = nlohmann::json;

int main(int, char* [])
{

    
    // Inicializa o mecanismo
    if (not engine::init(1920, 1080))
    {
        engine::close();
        return -1;
    }

    game::ConfigSelection mapSelection("./assets/maps");
    // Nenhum mapa encontrado
    if(mapSelection.getNumberOfFiles() == 0)
    {
        std::cout << "Erro: Nenhum arquivo de mapa (.rcmap) encontrado" << std::endl;
        engine::close();
        return -1;
    }


    
    bool quitSelection = false;
    bool started = false;

    std::vector<std::string> movementFiles;

    while(!started)
    {
        // Seleção do mapa
        while (!quitSelection)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_UP)
                    {
                        mapSelection.navigateUp();
                    }
                    else if (e.key.keysym.sym == SDLK_DOWN)
                    {
                        mapSelection.navigateDown();
                    }
                    else if (e.key.keysym.sym == SDLK_RETURN)
                    {
                        quitSelection = true;  // Finaliza a seleção ao pressionar Enter
                        break;
                    }
                    else if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        engine::close();  //  Permite fechar o jogo ao pressionar 'esc'
                        return 0;
                    }
                }
                else if (e.type == SDL_QUIT)
                {
                    engine::close();
                    return 0;
                }
            }
            engine::screen::clear();

            // Selecao do labirinto
            game::Maze mazePreview;
            try
            {
                mazePreview = game::Maze::loadMapfromFile(mapSelection.getSelectedMap());
                mazePreview.drawCentered(true); // Desenhe o preview do labirinto
                mapSelection.writeTextSelection(mapSelection.getSelectedMap());
                if(!mazePreview.hasMovementFiles())
                    mapSelection.writeErrorMsg("Esse mapa não possui arquivos de movimento");
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                return -1;
            }

            engine::screen::show();

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            {
                break;
            }
        }

        // Abertura do mapa
        std::ifstream selectedMapFile(mapSelection.getSelectedMap());

        if (!selectedMapFile.is_open())
        {
            throw std::invalid_argument("Failed to open file: " + mapSelection.getSelectedMap());
        }

        std::string rcmapHash;
        getline(selectedMapFile, rcmapHash);

        selectedMapFile.close();    

        game::ConfigSelection movementSelection("./assets/movements", rcmapHash);
        // Nenhum arquivo de movimento encontrado
        // TODO: Lógica da mensagem de erro
        if(movementSelection.getNumberOfFiles() == 0)
        {
            quitSelection = false;
            continue;
        }
        // Seleção da quantidade de ratos
        game::RatSelection ratSelection;
        quitSelection = false;

        GameDesign movementScreen(1920,1080);
    
        while (!quitSelection) {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_RIGHT) {
                        ratSelection.navigateRight();
                    } else if (e.key.keysym.sym == SDLK_LEFT) {
                        ratSelection.navigateLeft();
                    } else if (e.key.keysym.sym == SDLK_RETURN) {
                        quitSelection = true;  // Finaliza a seleção ao pressionar Enter
                        break;
                    } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                        engine::close();  // Permite fechar o jogo ao pressionar 'esc'
                        return 0;
                    }
                } else if (e.type == SDL_QUIT) {
                    engine::close();
                    return 0;
                }
            }

            engine::screen::clear();		
            movementScreen.draw();
            ratSelection.writeQuantitySelection();
            engine::screen::show();

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) 
            {
                break;
            }
        }

        // Seleção dos arquivos de movimento
        for (int i = 0; i < ratSelection.getSelectedQuantity(); ++i)
        {
            quitSelection = false;

            while (!quitSelection) {
                SDL_Event e;
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_UP) {
                            movementSelection.navigateUp();
                        } else if (e.key.keysym.sym == SDLK_DOWN) {
                            movementSelection.navigateDown();
                        } else if (e.key.keysym.sym == SDLK_RETURN) {
                            quitSelection = true;  // Finaliza a seleção ao pressionar Enter
                            break;
                        } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                            engine::close();  // Permite fechar o jogo ao pressionar 'esc'
                            return 0;
                        }
                    } else if (e.type == SDL_QUIT) {
                        engine::close();
                        return 0;
                    }
                }

                engine::screen::clear();
                movementScreen.draw();
                ratSelection.writeQuantitySelection();
                for (int j = 0; j < i; ++j) {
                    if (i > 0){
                        movementSelection.writeTextSelection(movementFiles[j], 63*(j+4));
                    }
                }
                movementSelection.writeTextSelection(movementSelection.getSelectedMap(), 63*(i+4));
                engine::screen::show();

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                    break;
                }
            }
            movementFiles.push_back(movementSelection.getSelectedMap());
            started = true;
        }
    }

    // Inicia o jogo com o mapa selecionado
    game::Maze maze;
    try
    {
        maze = game::Maze::loadMapfromFile(mapSelection.getSelectedMap());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    std::vector<RatInstance> rats;

    std::vector<std::string> ratImages = {
        "./assets/rat/whiteRat.png",
        "./assets/rat/brownRat.png",
        "./assets/rat/blackRat.png",
        "./assets/rat/redRat.png",
        "./assets/rat/greyRat.png",
        "./assets/rat/beigeRat.png",
    };

    std::vector<std::tuple<std::string, std::string, std::string>> mouseData;


    int maxMovements = -1;
    for (size_t i = 0; i < movementFiles.size(); ++i) {

        std::ifstream movementsFile(movementFiles[i]);

        // TODO: Tem no ratInstance, verificar se pode ser retirado de lá
        pJSON jsonFile = pJSON::parse(movementsFile);
        std::string ratName = jsonFile["ratName"];
        int n = jsonFile["movements"];
        int col = jsonFile["path"][0]["col"], row = jsonFile["path"][0]["row"];

        int validator = maze.validatorMovement(movementFiles[i], mapSelection.getSelectedMap());

        if (validator != -1) {
            RatInstance newRat(col, row, ratImages[i], movementFiles[i], mapSelection.getSelectedMap());
            rats.push_back(newRat);
        }
        
        if (validator == -1) {
            n = 1000;
        }

        if (validator == 1) {
            mouseData.push_back(std::make_tuple(ratName, std::to_string(999), ratImages[i]));
        } else {
            mouseData.push_back(std::make_tuple(ratName, std::to_string(n), ratImages[i]));
        }


        if (n > maxMovements and n != 1000) {
            maxMovements = n;
        }

        movementsFile.close();
    }


    GameDesign button(1920, 1080);
    // Loop principal
    bool quit = false;
    bool quitRanking = false;

    int totalMovements = maxMovements;

    int speed = 1;

    if (maxMovements == -1){
        quit = true;
    }
    while (!quit)
    {

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                quitRanking = true;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 750 && mouseX <= 950 && mouseY >= 1000 && mouseY <= 1030) {
                    speed = std::max(1, speed - 1);
                } else if (mouseX >= 970 && mouseX <= 1170 && mouseY >= 1000 && mouseY <= 1030) {
                    speed = std::min(50, speed + 1);
                }
            }
        }

        engine::screen::clear();


        maze.drawCentered();
        for (RatInstance& ratInstance : rats) {
            ratInstance.update(SDL_GetTicks()*speed);
            ratInstance.draw();
            if (ratInstance.getIndex() == maxMovements) {
                if (totalMovements != maxMovements + 100)
                    totalMovements++;
            }
        }

        if (totalMovements >= maxMovements + 100) {
            quit = true;
        }

        button.drawButtonSpeedy();
        engine::screen::show();

    }

    DrawRanking ranking(1920, 1080);
    while(!quitRanking) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quitRanking = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                quitRanking = true;  // Permite fechar o jogo ao pressionar 'esc'
                break;
            }
        }

        engine::screen::clear();

        ranking.drawRanking(mouseData);

        engine::screen::show();
    }


    engine::close();

    return 0;
}
