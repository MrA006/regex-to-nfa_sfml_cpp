#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

struct NFA {
    vector<string> states;
    vector<char> letters;
    vector<vector<string>> transition_function;
    vector<string> start_state;
    vector<string> final_state;
};

NFA nfa;

void addSymbolIfNew(const char& symbol) {
    if (find(nfa.letters.begin(), nfa.letters.end(), symbol) == nfa.letters.end()) {
        nfa.letters.push_back(symbol);
    }
}

void addToTransitionFunction(const NFAState* from, const char& symbol, const NFAState* to) {
    nfa.transition_function.push_back({ "Q" + to_string(from->num), string(1, symbol), "Q" + to_string(to->num) });
}

void arrange_transitions(NFAState* st, vector<NFAState*>& state_done) {
    if (find(state_done.begin(), state_done.end(), st) != state_done.end()) {
        return;
    }

    state_done.push_back(st);

    for (const auto& entry : st->next_state) {
        const char& symbol = entry.first;
        addSymbolIfNew(symbol);

        for (NFAState* ns : entry.second) {
            addToTransitionFunction(st, symbol, ns);
            string temp = "Q" + to_string(ns->num);
            if (find(nfa.states.begin(), nfa.states.end(), temp) == nfa.states.end()) {

                nfa.states.push_back(temp);
            }
            arrange_transitions(ns, state_done);
        }
    }
}


void final_st_dfs() {
    
    for (const string& st : nfa.states) {
        int count = 0;
        for (const vector<string>& val : nfa.transition_function) {
            //cout << st <<" = " << val[0] << " - " << val[2] << endl;
            
            if (val[0] == st && val[2] != st) {
                count += 1;
            }
        }
        if (count == 0 && find(nfa.final_state.begin(), nfa.final_state.end(), st) == nfa.final_state.end()) {
            //cout << "here" << endl;
            nfa.final_state.push_back(st);
        }
    }
}


void arrange_nfa(pair<NFAState*, NFAState*> fa) {
    nfa.states = { "Q" + to_string(fa.first->num) };
    vector<NFAState*> done;
    arrange_transitions(fa.first, done);


    nfa.start_state = { "Q" + to_string(fa.first->num) };
    final_st_dfs();
    cout << "Transitions :  " << endl;
    for (auto t : nfa.transition_function) {
        //for (auto t2 : t) {
            cout << t[0] <<" -> " << t[1]  << " -> " << t[2] << endl;
        //}
    }
    cout << " initial : ";
    for (auto a : nfa.start_state) {
        cout << a << " ";
    }
    cout << endl;
    cout << " final : ";
    for (auto a:nfa.final_state) {
        cout << a << " ";
    }
    cout << endl;

}