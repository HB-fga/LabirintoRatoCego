#include "engine.h"
#include "ratInstance.h"
#include "maze.h"
#include "rat.h"
#include "configSelection.h"
#include <iostream>

int main(int, char* [])
{
    // Inicializa o mecanismo
    if (not engine::init(1920, 1080))
    {
        engine::close();
        return -1;
    }

    // Seleção do mapa
    game::ConfigSelection mapSelection("./assets/maps");
    int loops = 1;

    bool quitSelection = false;

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

    // std::vector<char> movements;
    // try
    // {
    //     movements = maze.loadMovementsFromFile("./assets/movements/rat.txt");
    // }
    // catch (const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    //     return -1;
    // }
    // maze.setMovements(movements);

    std::vector<RatInstance> rats;

    std::vector<std::string> movementFiles = {
        "./assets/movements/rat.txt",
        "./assets/movements/rat2.txt",
    };

    for (const std::string& file : movementFiles) {
        RatInstance newRat(0, 0, file, mapSelection.getSelectedMap());
        rats.push_back(newRat);
    }


    // Loop principal
    bool quit = false;

    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;  // Permite fechar o jogo ao pressionar 'esc'
                break;
            }
        }

        engine::screen::clear();


        // maze.update(SDL_GetTicks());
        maze.drawCentered();
        for (RatInstance& ratInstance : rats) {
            ratInstance.update(SDL_GetTicks());
            ratInstance.draw();
        }
        engine::screen::show();
    }

    engine::close();

    return 0;
}
