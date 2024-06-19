#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <openssl/md5.h>
#include <openssl/evp.h>

using namespace std;
using pJSON = nlohmann::json;

const int P = 999; // Número máximo de movements permitidos

std::string md5(const std::string &str){
  unsigned char hash[MD5_DIGEST_LENGTH];

  EVP_Q_digest(NULL, "MD5", NULL, str.c_str(), str.size(), hash, NULL);

  std::stringstream ss;

  for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( hash[i] );

  return ss.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <caminho para o arquivo do mapa>" << endl;
        return 1;
    }

    string mapFilePath = argv[1];
    ifstream inputFile(mapFilePath);

    if (!inputFile) {
        cerr << "Nao foi possivel abrir o arquivo do mapa: " << mapFilePath << endl;
        return 1;
    }

    std::string line;
    std::string rcmapHash;
    std::string rcmapJson = "";

    getline(inputFile, rcmapHash);

    while(getline(inputFile, line))
    {
        rcmapJson = rcmapJson + line + "\n";
    }

    if (rcmapHash != md5(rcmapJson)) {
        cerr << "Nao foi possivel abrir o arquivo do mapa: " << mapFilePath << "\nO arquivo está corrompido ou não é suportado" << endl;
        return 1;
    }

    cerr << "==============================================" << endl;
    cerr << "=           LABIRINTO DO RATO CEGO           =" << endl;
    cerr << "==============================================\n" << endl;

    pJSON jsonFile = pJSON::parse(rcmapJson);

    int width = jsonFile["width"], height = jsonFile["height"];
    auto maze = jsonFile["map"];
    cerr << "Largura: " << width << " - Altura: " << height << endl;

    int numDecisionPoints = jsonFile["decisionCount"];
    cerr << "Numero de pontos de decisao: " << numDecisionPoints << endl;

    vector<pair<int, int>> decisionPoints;
    vector<string> possibleDirections;
    string directionsCurrent;
    int xInitial, yInitial;

    for(const auto& decision: jsonFile["decisions"]){
        int x = decision["row"], y = decision["col"];
        string directions = decision["moves"];
        decisionPoints.push_back({x, y});
        possibleDirections.push_back(directions);
        if (maze[x][y] == 4) {
            xInitial = x, yInitial = y;
            directionsCurrent = directions;
        }
    }

    // coordenadas finais
    int xFinal = jsonFile["exit"]["row"], yFinal = jsonFile["exit"]["col"];
    cerr << "Ponto Final: " << xFinal << " " << yFinal << endl;
    cerr << "==============================================\n" << endl;


    int xCurrent = xInitial, yCurrent = yInitial;
    string currentDirection;
    int movements = 1;

    // Vetor para armazenar o caminho percorrido
    vector<pair<int, int>> pathRat;
    pathRat.emplace_back(xCurrent, yCurrent);

    while(true){
        // Verifica se o rato ultrapassou o número máximo de movements
        if (movements == P){
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
            cerr << "O rato chegou ao ponto final em " << movements << " passos." << endl;
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
            cerr << "Posicao atual: " << xCurrent << " " << yCurrent << endl;
            cerr << "Possiveis direcoes: " << directionsCurrent << endl;
            cout << directionsCurrent << endl;

            string directionChosen;
            cin >> directionChosen;
            cerr << "Direcao escolhida: " << directionChosen << endl;

            currentDirection = directionChosen;

            if (directionsCurrent.find(currentDirection) != string::npos) {
                cerr << "OK - Movendo para o proximo ponto" << endl;
                movements++;
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
                cerr << "antes push" << endl;
                pathRat.push_back({xCurrent, yCurrent});
                cerr << "depois push" << endl;
            }
            else {
                cerr << "FALHA - Direcao invalida. Tente novamente." << endl;
            }
        } 
        else {
            movements++;
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

    // Construindo nome do arquivo de movimento
    string fileName = mapFilePath;
    fileName = fileName.substr(fileName.find_last_of("/") + 1);
    fileName = fileName.substr(0, fileName.find_last_of(".")) + "-";

    for (int i = 0; i < ratName.size(); i++) {
        if (ratName[i] != ' ') {
            fileName += ratName[i];
        }
    }

    // Arquivo de saída para o movimento do rato
    pJSON jsonOutput;

    jsonOutput["mapHash"] = rcmapHash;
    jsonOutput["ratName"] = ratName;
    jsonOutput["movements"] = movements;
    jsonOutput["path"] = pJSON::array();

    for(const auto& [row, col] : pathRat)
        jsonOutput["path"].push_back({{"row", row}, {"col", col}});

    ofstream outputFile("../assets/movements/" + fileName + ".json");
    outputFile << jsonOutput.dump(4) << endl;
    inputFile.close(); // Fecha o arquivo após a leitura.
    outputFile.close(); // Fecha o arquivo após a escrita.

    return 0;
}