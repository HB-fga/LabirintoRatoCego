#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>

using namespace std;

const int P = 999; // Número máximo de moviments permitidos


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <caminho para o arquivo do mapa>" << endl;
        return 1;
    }

    string mapFilePath = argv[1];
    ifstream inputFile(mapFilePath);

    if (!inputFile.is_open()) {
        cerr << "Nao foi possivel abrir o arquivo do mapa: " << mapFilePath << endl;
        return 1;
    }

    cerr << "==============================================" << endl;
    cerr << "=           LABIRINTO DO RATO CEGO           =" << endl;
    cerr << "==============================================\n" << endl;


    int width, height, cellSize;
    inputFile >> width >> height >> cellSize;
    cerr << "Largura: " << width << " - Altura: " << height << " - Tamanho da celula: " << cellSize << endl;

    vector<vector<int>> maze;
    for (int i = 0; i < height; i++) {
        vector<int> row;
        for (int j = 0; j < width; j++) {
            int cell;
            inputFile >> cell;
            row.push_back(cell);
        }
        maze.push_back(row);
    }

    int numDecisionPoints;
    inputFile >> numDecisionPoints;

    cerr << "Numero de pontos de decisao: " << numDecisionPoints << endl;

    vector<pair<int, int>> decisionPoints;
    vector<string> possibleDirections;

    string directionsCurrent;
    int xInitial, yInitial;
    for (int i = 0; i <= numDecisionPoints; i++) {
        int x, y;
        string directions;
        inputFile >> x >> y >> directions;
        decisionPoints.push_back({x, y});
        possibleDirections.push_back(directions);
        if (maze[x][y] == 4) {
            xInitial = x, yInitial = y;
            directionsCurrent = directions;
        }
    }

    // coordenadas finais
    int xFinal, yFinal;
    inputFile >> xFinal >> yFinal;
    cerr << "Ponto Final: " << xFinal << " " << yFinal << endl;
    cerr << "==============================================\n" << endl;


    int xCurrent = xInitial, yCurrent = yInitial;
    string currentDirection;
    int moviments = 1;

    // Vetor para armazenar o caminho percorrido
    vector<pair<int, int>> pathRat;
    pathRat.emplace_back(xCurrent, yCurrent);


    while(true){
        // Verifica se o rato ultrapassou o número máximo de moviments
        if (moviments == P){
            cout << "LOSE" << endl;
            cerr << "==============================================" << endl;
            cerr << "=        FIM DO LABIRINTO DO RATO CEGO       =" << endl;
            cerr << "==============================================\n" << endl;
            cerr << "O rato nao chegou ao ponto final em menos de 999 passos" << endl;
            break;
        }

        // Verifica se o rato chegou ao ponto final
        if (xCurrent == xFinal && yCurrent == yFinal) {
            cout << "WIN" << endl;
            cerr << "==============================================" << endl;
            cerr << "=        FIM DO LABIRINTO DO RATO CEGO       =" << endl;
            cerr << "==============================================\n" << endl;
            cerr << "O rato chegou ao ponto final em " << moviments << " passos." << endl;
            break;
        }

        if (maze[xCurrent][yCurrent] == 4 || maze[xCurrent][yCurrent] == 2) {

            // posição do vetor do decisionPoints atual
            int pos = 0;
            for (int i = 0; i < decisionPoints.size(); i++) {
                if (decisionPoints[i].first == xCurrent && decisionPoints[i].second == yCurrent) {
                    pos = i;
                    break;
                }
            }

            // directionsCurrent na posição pos
            directionsCurrent = possibleDirections[pos];
            cerr << "Possicao atual: " << xCurrent << " " << yCurrent << endl;
            cerr << "Possiveis direcoes: " << directionsCurrent << endl;
            cout << directionsCurrent << endl;

            string directionChosen;
            cin >> directionChosen;
            cerr << "Direcao escolhida: " << directionChosen << endl;

            currentDirection = directionChosen;

            if (directionsCurrent.find(currentDirection) != string::npos) {
                cerr << "OK - Movendo para o proximo ponto" << endl;
                moviments++;
                // Atualiza as coordenadas atuais
                if (currentDirection == "N") {
                    xCurrent--;
                }
                else if (currentDirection == "S") {
                    xCurrent++;
                }
                else if (currentDirection == "W") {
                    yCurrent--;
                }
                else if (currentDirection == "E") {
                    yCurrent++;
                }

                // Adiciona as coordenadas atuais ao pathRat
                pathRat.push_back({xCurrent, yCurrent});
            }
            else {
                cerr << "FALHA - Direcao invalida. Tente novamente." << endl;
            }
        } 
        else {
            moviments++;
            // Atualiza as coordenadas atuais
            if (currentDirection == "N") {
                xCurrent--;
            }
            else if (currentDirection == "S") {
                xCurrent++;
            }
            else if (currentDirection == "W") {
                yCurrent--;
            }
            else if (currentDirection == "E") {
                yCurrent++;
            }

            // Adiciona as coordenadas atuais ao pathRat
            pathRat.push_back({xCurrent, yCurrent});
        }
    }

    cerr << "\nEscolha o nome do rato (o nome nao deve conter espacos ou caracteres especiais)" << endl;
    string ratName;
    cin >> ratName;
    cerr << "Nome do rato: " << ratName << endl;

    // limpando o nome e juntando tudo para ficar como nome do arquivo
    string fileName = "";
    for (int i = 0; i < ratName.size(); i++) {
        if (ratName[i] != ' ') {
            fileName += ratName[i];
        }
    }

    // arquivo de saída para o movimento do rato
    ofstream outputFile("../assets/movements/" + fileName + ".txt");
    outputFile << ratName << endl;
    outputFile << moviments << endl;
    for (int i = 0; i < pathRat.size(); i++) {
        outputFile << pathRat[i].first << " " << pathRat[i].second << endl;
    }


    inputFile.close(); // Fecha o arquivo após a leitura.
    outputFile.close(); // Fecha o arquivo após a escrita.

    return 0;
}