#include "engine.h"
#include "ratInstance.h"
#include "maze.h"
#include "rat.h"
#include "gameDesign.h"
#include "configSelection.h"
#include "ratSelection.h"
#include "drawRanking.h"
#include "../libs/json.hpp"

#include <iostream>

using pJSON = nlohmann::json;

void decreaseSpeed(int* speed, int* oldSpeed)
{
    if(*speed != 0)
        *speed = std::max(1, *speed - 1);
    else
        *oldSpeed = std::max(1, *oldSpeed - 1);
}

void increaseSpeed(int* speed, int* oldSpeed)
{
    if(*speed != 0)
        *speed = std::min(50, *speed + 1);
    else
        *oldSpeed = std::min(50, *oldSpeed + 1);
}

void pause(int* speed, int* oldSpeed)
{
    *speed ? *oldSpeed = *speed, *speed = 0 : *speed = *oldSpeed;
}

void rewind(bool* reverse)
{
    *reverse = !*reverse;
}

int main(int, char* [])
{
    // Inicializa o mecanismo
    if (not engine::init())
    {
        engine::close();
        return -1;
    }

    game::ConfigSelection mapSelection("./assets/maps");
    // Nenhum mapa encontrado
    if(mapSelection.getNumberOfFiles() == 0)
    {
        std::cout << "Erro: Nenhum arquivo de mapa encontrado" << std::endl;
        engine::close();
        return -1;
    }
    
    bool quitSelection = false;
    bool changed = true;
    bool hasMoves = false;

    std::vector<std::string> movementFiles;

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
                    changed = true;
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    mapSelection.navigateDown();
                    changed = true;
                }
                else if (e.key.keysym.sym == SDLK_RETURN and hasMoves)
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
            if(changed)
            {
                changed = false;
                hasMoves = mazePreview.hasMovementFiles();
            }

            if(!hasMoves)
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

    pJSON jsonMapFile = pJSON::parse(selectedMapFile);
    std::string mapHash = jsonMapFile["mapHash"];
    selectedMapFile.close();    
    game::ConfigSelection movementSelection("./assets/movements", mapHash);

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
        }
        movementFiles.push_back(movementSelection.getSelectedMap());
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
        "./assets/spritesheets/whiteRat-Sheet.png",
        "./assets/spritesheets/brownRat-Sheet.png",
        "./assets/spritesheets/blackRat-Sheet.png",
        "./assets/spritesheets/redRat-Sheet.png",
        "./assets/spritesheets/greyRat-Sheet.png",
        "./assets/spritesheets/beigeRat-Sheet.png",
    };

    std::vector<std::tuple<std::string, std::string, std::string>> mouseData;


    int maxMovements = -1;
    for (size_t i = 0; i < movementFiles.size(); ++i) {

        std::ifstream movementsFile(movementFiles[i]);

        // TODO: Tem no ratInstance, verificar se pode ser retirado de lá
        pJSON jsonFile = pJSON::parse(movementsFile);
        std::string ratName = jsonFile["ratName"];
        int n = jsonFile["movements"];
        int ratActions = jsonFile["actions"];
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
            mouseData.push_back(std::make_tuple(ratName, std::to_string(ratActions), ratImages[i]));
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
    int oldSpeed;
    bool reverse = false;

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
            } else if (e.type == SDL_KEYDOWN and e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                quitRanking = true;
                break;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        decreaseSpeed(&speed, &oldSpeed);
                        break;

                    case SDLK_RIGHT:
                        increaseSpeed(&speed, &oldSpeed);
                        break; 

                    case SDLK_p:
                        pause(&speed, &oldSpeed);
                        break;

                    case SDLK_r:
                        rewind(&reverse);
                        break;
                } 
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= 800 && mouseX <= 840 && mouseY >= 1000 && mouseY <= 1030) {
                    decreaseSpeed(&speed, &oldSpeed);
                } else if (mouseX >= 1085 && mouseX <= 1125 && mouseY >= 1000 && mouseY <= 1030) {
                    increaseSpeed(&speed, &oldSpeed);
                } else if (mouseX >= 940 && mouseX <= 980 && mouseY >= 1040 && mouseY <= 1070) {
                    pause(&speed, &oldSpeed);
                } else if (mouseX >= 890 && mouseX <= 930 && mouseY >= 1040 && mouseY <= 1070) {
                    rewind(&reverse);
                }
            }
        }

        engine::screen::clear();


        maze.drawCentered();
        for (RatInstance& ratInstance : rats) {
            if(!reverse)
                ratInstance.update(SDL_GetTicks()*speed);
            else
                ratInstance.reverseUpdate(SDL_GetTicks()*speed);
            ratInstance.draw();
            if (ratInstance.getIndex() == maxMovements) {
                if (totalMovements != maxMovements + 100)
                    totalMovements++;
            }
        }

        if (totalMovements >= maxMovements + 100) {
            quit = true;
        }

        button.drawButtonSpeedy(speed);
        button.drawButtonReverse(reverse);
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