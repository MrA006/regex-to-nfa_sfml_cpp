#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>

using namespace std;

class Type {
public:
    int symbol = 1, concatenate = 2, star = 4, union_ = 3;
};

Type type;


struct ETree {
    int type;
    char value;
    ETree* left;
    ETree* right;

    ETree(int t, char v = '\0') : type(t), value(v), left(nullptr), right(nullptr) {}
};

ETree* make_exp_tree(const std::string& expression) {
    std::stack<ETree*> arr;
    for (char c : expression) {
        if (c == '+') {
            ETree* temp = new ETree(type.union_);
            temp->right = arr.top(); arr.pop();
            temp->left = arr.top(); arr.pop();
            arr.push(temp);
        }
        else if (c == '.') {
            ETree* temp = new ETree(type.concatenate);
            temp->right = arr.top(); arr.pop();
            temp->left = arr.top(); arr.pop();
            arr.push(temp);
        }
        else if (c == '*') {
            ETree* temp = new ETree(type.star);
            temp->left = arr.top(); arr.pop();
            arr.push(temp);
        }
        else if (c == '(' || c == ')') {
            continue;
        }
        else {
            arr.push(new ETree(type.symbol, c));
        }
    }
    return arr.top();
}


int count = 0;
struct NFAState {
    unordered_map<char, vector<NFAState*>> next_state;
    int num;
    NFAState() {
        num = ::count++;
    }
};

pair<NFAState*, NFAState*> compute_regex(ETree* exp_t);

pair<NFAState*, NFAState*> eval_symbol(char symbol) {
    NFAState* start = new NFAState();
    NFAState* end = new NFAState();
    start->next_state[symbol] = { end };
    return { start, end };
}

pair<NFAState*, NFAState*> do_concat(ETree* exp_t) {
    pair<NFAState*, NFAState*> left_result = compute_regex(exp_t->left);
    NFAState* left_start = left_result.first;
    NFAState* left_end = left_result.second;
    pair<NFAState*, NFAState*> right_result = compute_regex(exp_t->right);
    NFAState* right_start = right_result.first;
    NFAState* right_end = right_result.second;

    left_end->next_state['$'] = { right_start };
    return { left_start, right_end };
}

pair<NFAState*, NFAState*> do_union(ETree* exp_t) {
    NFAState* start = new NFAState();
    NFAState* end = new NFAState();
    pair<NFAState*, NFAState*> first_result = compute_regex(exp_t->left);
    NFAState* first_start = first_result.first;
    NFAState* first_end = first_result.second;

    pair<NFAState*, NFAState*> second_result = compute_regex(exp_t->right);
    NFAState* second_start = second_result.first;
    NFAState* second_end = second_result.second;

    start->next_state['$'] = { first_start, second_start };
    first_end->next_state['$'] = { end };
    second_end->next_state['$'] = { end };
    return { start, end };
}

pair<NFAState*, NFAState*> do_kleene_star(ETree* exp_t) {
    NFAState* start = new NFAState();
    NFAState* end = new NFAState();
    pair<NFAState*, NFAState*> starred_result = compute_regex(exp_t->left);
    NFAState* starred_start = starred_result.first;
    NFAState* starred_end = starred_result.second;
    start->next_state['$'] = { starred_start, end };
    starred_end->next_state['$'] = { starred_start, end };
    return { start, end };
}

pair<NFAState*, NFAState*> compute_regex(ETree* exp_t) {
    if (exp_t->type == type.concatenate) {
        return do_concat(exp_t);
    }
    else if (exp_t->type == type.union_) {
        return do_union(exp_t);
    }
    else if (exp_t->type == type.star) {
        return do_kleene_star(exp_t);
    }
    else {
        return eval_symbol(exp_t->value);
    }
}

void print_nfa(NFAState* start_state) {
    stack<NFAState*> stack;
    set<NFAState*> visited;
    stack.push(start_state);
    while (!stack.empty()) {
        NFAState* state = stack.top();
        stack.pop();
        visited.insert(state);
        cout << "State: " << state->num << endl;
        for (auto& transition : state->next_state) {
            char symbol = transition.first;
            for (NFAState* next_state : transition.second) {
                if (visited.find(next_state) == visited.end()) {
                    cout << "Transition on '" << symbol << "': " << state->num << " -> " << next_state->num << endl;
                    stack.push(next_state);
                }
            }
        }
    }
}