#include "mapRender.h"
#include "engine.h"

MapRenderer::MapRenderer(int screenWidth, int screenHeight, int mazeWidth, int mazeHeight)
    : SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight), MAZE_WIDTH(mazeWidth),
      MAZE_HEIGHT(mazeHeight), CELL_SIZE(60) {

    buttonWidth = 200;
    buttonHeight = 30;
    buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
    buttonY = SCREEN_HEIGHT - 80;

    mapMaze = new CellType*[MAZE_HEIGHT];
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        mapMaze[i] = new CellType[MAZE_WIDTH]();
    }
}

MapRenderer::~MapRenderer() {
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        delete[] mapMaze[i];
    }
    delete[] mapMaze;
}


void MapRenderer::saveMapToFile(const std::string& filePath) {
    std::ofstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    file << MAZE_WIDTH << " " << MAZE_HEIGHT << " " << CELL_SIZE << "\n";

    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            int cellValue = 0;

            if (mapMaze[row][col] == CELL_PATH) {
                cellValue = 1;
            } else if (mapMaze[row][col] == CELL_DECISION) {
                cellValue = 2;
            } else if (mapMaze[row][col] == CELL_EXIT) {
                cellValue = 3;
            }

            file << cellValue << " ";
        }
        file << "\n";
    }

    int decisionCount = 0;
    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            if (mapMaze[row][col] == CELL_DECISION) {
                decisionCount++;
            }
        }
    }

    // Print the number of decision points
    file << decisionCount << "\n";

    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            if (mapMaze[row][col] == CELL_DECISION) {
                file << row << " " << col << ' ';

                for (int i = row - 1; i >= 0; --i)
                    if (mapMaze[i][col] == CELL_DECISION || mapMaze[i][col] == CELL_EXIT || mapMaze[i][col] == CELL_START)
                    {
                        file << "N";
                        break;
                    }

                for (int i = row + 1; i < MAZE_HEIGHT; ++i)
                    if (mapMaze[i][col] == CELL_DECISION)
                    {
                        file << "S";
                        break;
                    }

                for (int j = col + 1; j < MAZE_WIDTH; ++j)
                    if (mapMaze[row][j] == CELL_DECISION)
                    {
                        file << "E";
                        break;
                    }

                for (int j = col - 1; j >= 0; --j)
                    if (mapMaze[row][j] == CELL_DECISION)
                    {
                        file << "W";
                        break;
                    }

                file << '\n';
            }
        }
    }

    // Coordinates of the exit
    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            if (mapMaze[row][col] == CELL_EXIT) {
                file << row << " " << col << "\n";
                break;
            }
        }
    }

    file.close();
}

void MapRenderer::updateCellColor(int x, int y) {
    int offsetX = (SCREEN_WIDTH - MAZE_WIDTH * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - MAZE_HEIGHT * CELL_SIZE) / 2;

    // Converta as coordenadas do clique para coordenadas de células no mapa
    x -= offsetX;
    y -= offsetY;

    // Verifique se o clique está dentro do mapa
    if (x >= 0 && x < MAZE_WIDTH * CELL_SIZE && y >= 0 && y < MAZE_HEIGHT * CELL_SIZE) {
        int cellX = x / CELL_SIZE;
        int cellY = y / CELL_SIZE;

        // Printe as coordenadas (x, y)
        // std::cout << "Clicked cell coordinates: (" << cellX << ", " << cellY << ")" << std::endl;

        // Atualize a cor da célula com base na regra
        if (mapMaze[cellY][cellX] == CELL_EMPTY) {
            mapMaze[cellY][cellX] = CELL_DECISION;
        } else if (mapMaze[cellY][cellX] == CELL_DECISION) {
            mapMaze[cellY][cellX] = CELL_PATH;
        } else if (mapMaze[cellY][cellX] == CELL_PATH) {
            mapMaze[cellY][cellX] = CELL_EXIT;
        } else if (mapMaze[cellY][cellX] == CELL_EXIT) {
            mapMaze[cellY][cellX] = CELL_FORBIDDEN;
        } else if (mapMaze[cellY][cellX] == CELL_FORBIDDEN) {
            mapMaze[cellY][cellX] = CELL_EMPTY;
        }
    }
}

void MapRenderer::generateMaze() {
    engine::screen::clear();

    auto titleGererateTexture = engine::loadTexture("./assets/texts/generateMapTitle.png");
    if (!titleGererateTexture) {
        return;
    }
    SDL_Rect destRectTitle{ (SCREEN_WIDTH - 800) / 2, 30, 800, 50 };
    SDL_RenderCopy(engine::getRenderer(), titleGererateTexture.get(), 0, &destRectTitle);

    // Centraliza o mapa na tela
    int offsetX = (SCREEN_WIDTH - MAZE_WIDTH * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - MAZE_HEIGHT * CELL_SIZE) / 2;

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            SDL_Color color;
            switch (mapMaze[y][x]) {
                case CELL_DECISION:
                    color = { 0, 0, 255, 255 }; // Azul
                    break;
                case CELL_PATH:
                    color = { 255, 0, 0, 255 }; // Vermelho
                    break;
                case CELL_EXIT:
                    color = { 0, 255, 0, 255 }; // Verde
                    break;
                case CELL_FORBIDDEN:
                    color = { 0, 0, 0, 255 }; // Preto
                    break;
                default:
                    color = { 255, 255, 255, 255 }; // Branco
                    break;
            }
            engine::draw::rect(x * CELL_SIZE + offsetX, y * CELL_SIZE + offsetY, CELL_SIZE, CELL_SIZE, color);

            // Desenhe uma borda preta em cada célula
            SDL_Color borderColor = { 0, 0, 0, 255 }; // Preto
            engine::draw::rect(x * CELL_SIZE + offsetX, y * CELL_SIZE + offsetY, CELL_SIZE, 1, borderColor); // Linha superior
            engine::draw::rect(x * CELL_SIZE + offsetX, (y + 1) * CELL_SIZE - 1 + offsetY, CELL_SIZE, 1, borderColor); // Linha inferior
            engine::draw::rect(x * CELL_SIZE + offsetX, y * CELL_SIZE + offsetY, 1, CELL_SIZE, borderColor); // Linha esquerda
            engine::draw::rect((x + 1) * CELL_SIZE - 1 + offsetX, y * CELL_SIZE + offsetY, 1, CELL_SIZE, borderColor); // Linha direita
        }
    }


    auto buttonGererateTexture = engine::loadTexture("./assets/texts/generateMapButton.png");
    if (!buttonGererateTexture) {
        return;
    }
    SDL_Rect destRectButton{ buttonX, buttonY, buttonWidth, buttonHeight };
    SDL_RenderCopy(engine::getRenderer(), buttonGererateTexture.get(), 0, &destRectButton);

    engine::screen::show();
}
