#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GEBR.hpp"
using namespace std;

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
GEBR<State, Action, Properties, InformationSet>::GEBR(Game<State, Action, Properties, InformationSet> *game)
 : game(game)
{
    int information_sets = game -> discover_information_sets();
    t.resize(information_sets, vector<double>(0));
    b.resize(information_sets, vector<double>(0));
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
void GEBR<State, Action, Properties, InformationSet>::discover_tree(int d) {
    if(game -> terminal_state()) {
        return;
    }
    int I = game -> information_set_id();
    vector<Action> actions = game -> actions();
    int total_actions = actions.size();
    t[I].resize(total_actions, 0);
    b[I].resize(total_actions, 0);
    depths[game -> player() - 1].push_back(d);
    for (auto action : actions) {
        game -> update_state(action);
        discover_tree(d+1);
        game -> revert_state();
    }
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
void GEBR<State, Action, Properties, InformationSet>::pass1() {
    game -> initial_state();
    while(game -> valid_state()) {
        discover_tree(0);
        game -> next_state();
    }
    for (int i = 0; i < 2; i++){
        sort(depths[i].begin(), depths[i].end());
    }
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
double GEBR<State, Action, Properties, InformationSet>::pass2(int i, int d, int l, double pi) {
    if(game -> terminal_state()) {
        return game -> utility(i);
    }
    int I = game -> information_set_id();
    double v = 0;
    vector <Action> actions = game -> actions();
    if (game -> player() == i && l > d) {
        int a = 0;
        double max_value = t[I][a]/b[I][a];
        for (int k = 0; k < (int) actions.size(); k++) {
            if (t[I][k]/b[I][k] > max_value) {
                a = k;
                max_value = t[I][k]/b[I][k];
            }
        }
        game -> update_state(actions[i]);
        v = pass2(i, d, l+1, pi);
        game -> revert_state();
        return v;
    }
    int a = 0;
    for (auto action : actions) {
        double pi2 = pi;
        if (game -> player != i){
            pi2 *= (*sigma)[I][a];
        }
        game -> update_state(action);
        double v2 = pass2(i, d, l+1, pi2); 
        game -> revert_state();
        if (game -> player() != i) {
            v += (*sigma)[I][a]*v2;
        } else if(l == d) {
            t[I][a] += v2*pi;
            b[I][a] += pi;
        }
        a++;
    }
    return v;
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
double GEBR<State, Action, Properties, InformationSet>::best_response(int i) {
    double v;
    int k = 0;
    for (int j = depths[i].size() - 1; j >= 0; j--) {
        v = 0;
        k = 0;
        game -> first_initial_state();
        while(game -> valid_state()) {
            v += pass2(i, depths[j], 0, 1);
            game -> next_state();
            k++;
        }
    }
    return v/k;
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
double GEBR<State, Action, Properties, InformationSet>::explotability(vector<vector<double>>*strategy) {
    sigma = strategy;
    return best_response(0) + best_response(1);
}