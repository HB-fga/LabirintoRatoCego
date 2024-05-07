#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

#include "mapRender.h"
#include "engine.h"

using pJSON = nlohmann::json;

MapRenderer::MapRenderer(int screenWidth, int screenHeight, int initialMazeWidth, int initialMazeHeight)
    : SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight), MAZE_WIDTH(initialMazeWidth),
      MAZE_HEIGHT(initialMazeHeight), CELL_SIZE(60) {

    buttonWidth = 200;
    buttonHeight = 30;
    buttonXgererated = (SCREEN_WIDTH - buttonWidth) / 2;
    buttonYgererated = SCREEN_HEIGHT - 80;

    buttonXreset = (SCREEN_WIDTH - MAZE_WIDTH * CELL_SIZE) / 2;
    buttonYreset = SCREEN_HEIGHT - 80;

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


void MapRenderer::increaseMazeWidth() {
    if (MAZE_WIDTH < 30)
        MAZE_WIDTH++;
}

void MapRenderer::decreaseMazeWidth() {
    if (MAZE_WIDTH > 1)
        MAZE_WIDTH--;
}

void MapRenderer::increaseMazeHeight() {
    if (MAZE_HEIGHT < 15)
        MAZE_HEIGHT++;
}

void MapRenderer::decreaseMazeHeight() {
    if (MAZE_HEIGHT > 1)
        MAZE_HEIGHT--;
}
void MapRenderer::saveMapToFile(const std::string& filePath){
    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color grayLightColor{ 192, 192, 192, 255 };

    TTF_Font* font30p = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 30);
    if (font30p == nullptr) {
        return;
    }
    engine::renderText("Mapa criado!", buttonXgererated-65, buttonYgererated+40, font30p, whiteColor);
    engine::screen::show();
    SDL_Delay(1300);
    engine::renderText("Mapa criado!", buttonXgererated-65, buttonYgererated+40, font30p, grayLightColor);

    pJSON jsonFile;
    std::ofstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    jsonFile["height"] = MAZE_HEIGHT;
    jsonFile["width"] = MAZE_WIDTH;
    jsonFile["cellSize"] = CELL_SIZE;

    // Temporary
    std::vector<std::vector<int>> map;
    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        std::vector<int> line;
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            int cellValue = 0;
            if (mapMaze[row][col] == CELL_PATH) {
                cellValue = 1;
            } else if (mapMaze[row][col] == CELL_DECISION) {
                cellValue = 2;
            } else if (mapMaze[row][col] == CELL_EXIT) {
                cellValue = 3;
            } else if (mapMaze[row][col] == CELL_START) {
                cellValue = 4;
            }
            line.push_back(cellValue);
        }
        map.push_back(line);
    }

    jsonFile["map"] = map;

    int decisionCount = 0;
    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            if (mapMaze[row][col] == CELL_DECISION) {
                decisionCount++;
            }
        }
    }

    jsonFile["decisionCount"] = decisionCount;


    jsonFile["decisions"] = pJSON::array();
    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            if (mapMaze[row][col] == CELL_DECISION || mapMaze[row][col] == CELL_START) {
                std::string s;

                for (int i = row - 1; i >= 0; --i) {
                    if (mapMaze[i][col] == CELL_EMPTY || mapMaze[i][col] == CELL_FORBIDDEN) break;
                    if (mapMaze[i][col] == CELL_DECISION || mapMaze[i][col] == CELL_EXIT || mapMaze[i][col] == CELL_START)
                    {
                        s.append("N");
                        break;
                    }
                }

                for (int i = row + 1; i < MAZE_HEIGHT; ++i) {
                    if (mapMaze[i][col] == CELL_EMPTY || mapMaze[i][col] == CELL_FORBIDDEN) break;
                    if (mapMaze[i][col] == CELL_DECISION || mapMaze[i][col] == CELL_EXIT || mapMaze[i][col] == CELL_START)
                    {
                        s.append("S");
                        break;
                    }
                }

                for (int j = col + 1; j < MAZE_WIDTH; ++j) {
                    if (mapMaze[row][j] == CELL_EMPTY || mapMaze[row][j] == CELL_FORBIDDEN) break; 
                    if (mapMaze[row][j] == CELL_DECISION || mapMaze[row][j] == CELL_EXIT || mapMaze[row][j] == CELL_START)
                    {
                        s.append("E");
                        break;
                    }
                }

                for (int j = col - 1; j >= 0; --j) {
                    if (mapMaze[row][j] == CELL_EMPTY || mapMaze[row][j] == CELL_FORBIDDEN) break; 
                    if (mapMaze[row][j] == CELL_DECISION || mapMaze[row][j] == CELL_EXIT || mapMaze[row][j] == CELL_START)
                    {
                        s.append("W");
                        break;
                    }
                }
                jsonFile["decisions"].push_back({{"row", row}, {"col", col}, {"moves", s}});
                
            }
        }
    }

    // Coordinates of the exit
    for (int row = 0; row < MAZE_HEIGHT; ++row) {
        for (int col = 0; col < MAZE_WIDTH; ++col) {
            if (mapMaze[row][col] == CELL_EXIT) {
                jsonFile["exit"] ={{"col", col}, {"row", row}};
                break;
            }
        }
    }
    
    file << jsonFile.dump(4) << std::endl;

    TTF_CloseFont(font30p);
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

        // Atualize a cor da célula com base na regra
        if (mapMaze[cellY][cellX] == CELL_EMPTY) {
            mapMaze[cellY][cellX] = CELL_DECISION;
        } else if (mapMaze[cellY][cellX] == CELL_DECISION) {
            mapMaze[cellY][cellX] = CELL_PATH;
        } else if (mapMaze[cellY][cellX] == CELL_PATH) {
            mapMaze[cellY][cellX] = CELL_EXIT;
        } else if (mapMaze[cellY][cellX] == CELL_EXIT) {
            mapMaze[cellY][cellX] = CELL_START;
        } else if (mapMaze[cellY][cellX] == CELL_START) {
            mapMaze[cellY][cellX] = CELL_FORBIDDEN;
        } else if (mapMaze[cellY][cellX] == CELL_FORBIDDEN) {
            mapMaze[cellY][cellX] = CELL_EMPTY;
        } 
    }
}

void MapRenderer::generateMaze() {
    engine::screen::clear();

    int offsetX = (SCREEN_WIDTH - MAZE_WIDTH * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - MAZE_HEIGHT * CELL_SIZE) / 2;

    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 0 };
    SDL_Color grayDarkColor{ 64, 64, 64, 255 };
    SDL_Color grayLightColor{ 192, 192, 192, 255 };

    engine::draw::rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, grayLightColor);
    engine::draw::rect(0, 0, SCREEN_WIDTH, 60, grayDarkColor);
    engine::draw::rect(10, 10, SCREEN_WIDTH - 20, 40, blackColor);

    TTF_Font* font30p = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 30);
    if (font30p == nullptr) {
        return;
    }
    engine::renderText("Criador de mapa Labirinto do Rato Cego", SCREEN_WIDTH / 4 - 95, 20, font30p, whiteColor);
    TTF_CloseFont(font30p);

    TTF_Font* font15p = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 15);
    if (font15p == nullptr) {
        return;
    }
    engine::renderText("Controle o tamanho do labirinto com as setas", 60, 70, font15p, whiteColor);
    engine::renderText(std::to_string(MAZE_HEIGHT) + "x" + std::to_string(MAZE_WIDTH), SCREEN_WIDTH / 2 - 35, 70, font15p, whiteColor);
    TTF_CloseFont(font15p);


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
                case CELL_START:
                    color = { 255, 255, 0, 255 }; // amarelo
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

    engine::draw::rect(buttonXgererated-5, buttonYgererated-5, buttonWidth+10, buttonHeight+10, grayDarkColor);
    engine::draw::rect(buttonXgererated, buttonYgererated, buttonWidth, buttonHeight, blackColor);

    TTF_Font* font18p = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 18);
    if (font18p == nullptr) {
        return;
    }

    engine::renderText("Gerar Mapa", buttonXgererated+10, buttonYgererated+5, font18p, whiteColor);

    engine::draw::rect(buttonXreset-5, buttonYreset-5, buttonWidth+10, buttonHeight+10, grayDarkColor);
    engine::draw::rect(buttonXreset, buttonYreset, buttonWidth, buttonHeight, blackColor);

    engine::renderText("Resetar", buttonXreset+40, buttonYreset+5, font18p, whiteColor);
    TTF_CloseFont(font18p);


    // Legenda
    int legendOffsetX = SCREEN_WIDTH - 500;  // Offset horizontal da legenda
    int legendOffsetY = SCREEN_HEIGHT - 80;  // Offset vertical da legenda
    int legendSquareSize = 20;  // Tamanho do quadrado na legenda
    int numColumns = 3;  // Número de colunas na legenda

    // Defina as cores e os significados
    std::vector<std::pair<SDL_Color, std::string>> legendColors = {
        { { 0, 0, 255, 255 }, "Decisao" },  // Azul
        { { 255, 0, 0, 255 }, "Caminho" },  // Vermelho
        { { 0, 255, 0, 255 }, "Saida" },  // Verde
        { { 255, 255, 0, 255 }, "Inicio" },  // Amarelo
        { { 0, 0, 0, 255 }, "Proibido" },  // Preto
        { { 255, 255, 255, 255 }, "Vazio" }  // Branco
    };

    // Desenha os quadrados e seus significados
    for (int i = 0; i < legendColors.size(); ++i) {
        SDL_Color color = legendColors[i].first;
        std::string label = legendColors[i].second;
        int row = i / numColumns;  // Calcular a linha atual
        int col = i % numColumns;  // Calcular a coluna atual
        int posX = legendOffsetX + col * (legendSquareSize + 150);
        int posY = legendOffsetY + row * (legendSquareSize + 25);
        engine::draw::rect(posX, posY, legendSquareSize, legendSquareSize, color);
        TTF_Font* fontLegend = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 15);
        engine::renderText(label, posX + legendSquareSize + 5, posY + 3, fontLegend, blackColor);
        TTF_CloseFont(fontLegend);
    }

    engine::screen::show();
}

void MapRenderer::resetMap() {
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            mapMaze[i][j] = CELL_EMPTY;
        }
    }
}
