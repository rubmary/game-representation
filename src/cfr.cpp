#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/CFR.cpp"
#include "games/KuhnPoker.hpp"
#include "games/OCP.hpp"
using namespace std;
int iterations = 1000000;
#define EPS 1e-5

void cfr_kuhn(ostream& os_regret, ostream& os_strategy, ostream& os_inf_sets) {
    using namespace kuhn_poker;
    KuhnPoker kuhn_poker;
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&kuhn_poker}, EPS);
    cfr.train(iterations, os_regret);
    cfr.print_strategy(os_strategy);
    kuhn_poker.print_information_sets(os_inf_sets);
}

void cfr_ocp(ostream& os_regret, ostream& os_strategy, ostream& os_inf_sets) {
    using namespace ocp;
    OCP ocp(3);
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&ocp}, EPS);
    cfr.train(iterations, os_regret);
    cfr.print_strategy(os_strategy);
    ocp.print_information_sets(os_inf_sets);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if(argc < 2) {
        cout << "Debes introducir el nombre del juego";
        cout << endl;
        return 0;
    }

    string game = argv[1];
    string path_regret = "regret/" + game + "/regret.txt";
    string path_strategy = "results/" + game + "/strategy.txt";
    string path_inf_sets = "results/" + game + "/information_sets.txt";
    ofstream os_regret(path_regret.c_str());
    ofstream os_strategy(path_strategy.c_str());
    ofstream os_inf_sets(path_inf_sets.c_str());

    if(game == "KuhnPoker"){
        cfr_kuhn(os_regret, os_strategy, os_inf_sets);
    } else if(game == "OCP"){
        cfr_ocp(os_regret, os_strategy, os_inf_sets);
    } else if(game == "Dudo") {
        cout << "En construccion" << endl;
    } else if (game == "Domino") {
        cout << "En construccion" << endl;
    } else {
        cout << "Error" << endl;
        cout << "Los juegos validos son:" << endl;
        cout << "\tKuhnPoker" << endl;
        cout << "\tOCP" << endl;
        cout << "\tDudo" << endl;
        cout << "\tDomino" << endl;
    }
}
