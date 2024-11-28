#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <set>

using namespace std;
using pJSON = nlohmann::json;

const int MAX = 999; // Número máximo de movements permitidos
const bool DEBUG = true; // Controle de exibição de informação de Debug

// Função de cálculo de Hash MD5
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

    // Extraindo o Json sem a linha "mapHash"
    string jsonMapContent = "";
    string jsonFoundHash = "";
    for(std::string line; getline(inputFile, line);)
    {
        if (line.find("mapHash") == std::string::npos) 
        {
            jsonMapContent = jsonMapContent + line + "\n";
        }
        else
        {
            int last = line.find_last_of("\"");
            jsonFoundHash = line.substr(last-32,32);
        }
    }

    if (jsonFoundHash != md5(jsonMapContent)) {
        cerr << "Nao foi possivel abrir o arquivo do mapa: " << mapFilePath << "\nO arquivo está corrompido ou não é suportado" << endl;
        return 1;
    }

    cerr << "==============================================" << endl;
    cerr << "=           LABIRINTO DO RATO CEGO           =" << endl;
    cerr << "==============================================" << endl;

    pJSON jsonFile = pJSON::parse(jsonMapContent);

    int width = jsonFile["width"], height = jsonFile["height"];
    if(DEBUG) cerr << "Largura: " << width << " - Altura: " << height << endl;
    int numDecisionPoints = jsonFile["decisionCount"];
    if(DEBUG) cerr << "Numero de pontos de decisao: " << numDecisionPoints << endl;
    auto maze = jsonFile["map"];

    vector<pair<int, int>> decisionPoints;
    vector<string> possibleDirections;
    set<pair<int, int>> markedCells;
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
    if(DEBUG) cerr << "Ponto Final: " << xFinal << " " << yFinal << endl;
    if(DEBUG) cerr << "==============================================\n" << endl;

    int xCurrent = xInitial, yCurrent = yInitial;
    string currentAction;
    int movements = 1;

    // Vetor para armazenar o caminho percorrido
    vector<pair<int, int>> pathRat;
    pathRat.emplace_back(xCurrent, yCurrent);

    while(true){
        // Verifica se o rato ultrapassou o número máximo de movements
        if (movements == MAX){
            cout << "LOSE" << endl;
            cerr << "==============================================" << endl;
            cerr << "=        FIM DO LABIRINTO DO RATO CEGO       =" << endl;
            cerr << "==============================================\n" << endl;
            if(DEBUG) cerr << "O rato nao chegou ao ponto final em menos de 999 passos" << endl << endl;
            break;
        }

        // Verifica se o rato chegou ao ponto final
        if (xCurrent == xFinal && yCurrent == yFinal) {
            cout << "WIN" << endl;
            cerr << "==============================================" << endl;
            cerr << "=        FIM DO LABIRINTO DO RATO CEGO       =" << endl;
            cerr << "==============================================\n" << endl;
            if(DEBUG) cerr << "O rato chegou ao ponto final em " << movements << " passos." << endl << endl;
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
            if(DEBUG) cerr << "Posicao atual: " << xCurrent << " " << yCurrent << endl;
            if(DEBUG) cerr << "Possiveis direcoes: " << directionsCurrent << endl;
            if(DEBUG) cerr << "Celulas marcadas: ";
            for (auto cell : markedCells) {
                cerr << "(" << cell.first << ", " << cell.second <<") ";
            }
            cerr << endl;

            // Envia as direções possíveis para o código do jogador
            cout << directionsCurrent << endl;

            string actionChosen;
            cin >> actionChosen;
            if(DEBUG) cerr << "Movimento escolhido: " << actionChosen << endl;

            currentAction = actionChosen;

            // Ajustando opções de ação disponíveis
            directionsCurrent = directionsCurrent + "+?";

            if (directionsCurrent.find(currentAction) != string::npos) {
                // Atualiza as coordenadas atuais
                if (currentAction == "N") {
                    xCurrent--;
                }
                else if (currentAction == "S") {
                    xCurrent++;
                }
                else if (currentAction == "W") {
                    yCurrent--;
                }
                else if (currentAction == "E") {
                    yCurrent++;
                }
                else if (currentAction == "+") {
                    markedCells.emplace(xCurrent, yCurrent);
                }
                else if (currentAction == "?") {
                    if(DEBUG) cerr << (markedCells.count(make_pair(xCurrent, yCurrent)) ? "Marcada!\n" : "Não marcada!\n");
                    cout << (markedCells.count(make_pair(xCurrent, yCurrent)) ? "YES\n" : "NO\n");
                }

                if(DEBUG) cerr << "========== OK - Movendo para o proximo ponto" << endl;
                movements++;

                // Adiciona as coordenadas atuais ao pathRat
                pathRat.push_back({xCurrent, yCurrent});
            }
            else {
                if(DEBUG) cerr << "========== FALHA - Direcao invalida. Tente novamente." << endl;
            }
        } 
        else {
            movements++;
            // Atualiza as coordenadas atuais
            if (currentAction == "N") {
                xCurrent--;
            }
            else if (currentAction == "S") {
                xCurrent++;
            }
            else if (currentAction == "W") {
                yCurrent--;
            }
            else if (currentAction == "E") {
                yCurrent++;
            }

            // Adiciona as coordenadas atuais ao pathRat
            pathRat.push_back({xCurrent, yCurrent});
        }
    }

    // TODO: Melhorar tratamento da string de nome do rato
    cerr << "Escolha o nome do rato (o nome nao deve conter espacos ou caracteres especiais)" << endl;
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

    jsonOutput["mapHash"] = jsonFoundHash;
    jsonOutput["ratName"] = ratName;
    jsonOutput["movements"] = movements;
    jsonOutput["path"] = pJSON::array();

    for(const auto& [row, col] : pathRat)
        jsonOutput["path"].push_back({{"row", row}, {"col", col}});

    string movHash = md5(jsonOutput.dump(4));
    jsonOutput["movHash"] = movHash;

    ofstream outputFile("../assets/movements/" + fileName + ".json");

    outputFile << jsonOutput.dump(4);
    inputFile.close(); // Fecha o arquivo após a leitura.
    outputFile.close(); // Fecha o arquivo após a escrita.

    return 0;
}