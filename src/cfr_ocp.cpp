#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/CFR.cpp"
#include "games/OCP/OCP.hpp"
using namespace std;

int iterations = 1000000;

int main(int argc, char **argv) {

    ofstream file_strt(argv[1]);
    ofstream file_rgrt(argv[2]);

    OCP ocp(3);
    CFR<State, Action, Properties, InformationSet> cfr({&ocp});
    vector<vector<double>> regret     = cfr.train(iterations);
    cfr.train(iterations);
    vector <vector <double>> strategy   = cfr.average_strategy();

    int M = strategy.size();
    file_strt << M << endl;
    for (int i = 0; i < M; i++) {
        int N = strategy[i].size();
        file_strt << N << ' ';
        for (int j = 0; j < N; j++)
            file_strt << fixed << setprecision(5) << strategy[i][j] << ' ';
        file_strt << endl;
    }

    int T = regret.size();
    for (int t = 1; t < T; t++){
        file_rgrt << regret[t][0] << ' ';
        file_rgrt << regret[t][1] << ' ';
        file_rgrt << regret[t][0] + regret[t][1] << endl;
    }
}
