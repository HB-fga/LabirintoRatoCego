#include <iostream>
#include <string>

using namespace std;
int main() {
    string options;
    srand(time(NULL));
    while (true) {
        // recebe as opções do outro código
        cin >> options;

        if (options == "LOSE" || options == "WIN") {
            break;
        }

        cout << options[rand()%options.size()] << endl;

    }

    string ratName = "ratoAleatorioCPP";

    cout << ratName << endl;

    return 0;
}
