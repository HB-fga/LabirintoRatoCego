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

        options = options + "+?";
        char chosenOption = options[rand()%options.size()];

        cout << chosenOption << endl;

        if(chosenOption == '?')
        {
            string markAnswer;
            cin >> markAnswer;
        }

    }

    string ratName = "ratoAleatorioCPP";

    cout << ratName << endl;

    return 0;
}
