#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<char> non_symbols = { '+', '*', '.', '(', ')' };

string add_concat(const string& regular_expression) {
    string res;
    for (size_t i = 0; i < regular_expression.size() - 1; i++) {
        res += regular_expression[i];
        if (find(non_symbols.begin(), non_symbols.end(), regular_expression[i]) == non_symbols.end()) {
            if (find(non_symbols.begin(), non_symbols.end(), regular_expression[i + 1]) == non_symbols.end() || regular_expression[i + 1] == '(') {
                res += '.';
            }
        }
        if (regular_expression[i] == ')' && regular_expression[i + 1] == '(') {
            res += '.';
        }
        if (regular_expression[i] == '*' && regular_expression[i + 1] == '(') {
            res += '.';
        }
        if (regular_expression[i] == '*' && find(non_symbols.begin(), non_symbols.end(), regular_expression[i + 1]) == non_symbols.end()) {
            res += '.';
        }
        if (regular_expression[i] == ')' && find(non_symbols.begin(), non_symbols.end(), regular_expression[i + 1]) == non_symbols.end()) {
            res += '.';
        }
    }
    res += regular_expression.back();
    return res;
}

int precedence(char a, char b) {
    vector<char> arr = { '+', '.', '*' };
    auto it_a = find(arr.begin(), arr.end(), a);
    auto it_b = find(arr.begin(), arr.end(), b);
    return it_a - arr.begin() > it_b - arr.begin();
}

string compute_postfix(const string& expression) {
    vector<char> arr;
    string res;

    for (char c : expression) {
        if (find(non_symbols.begin(), non_symbols.end(), c) == non_symbols.end() || c == '*') {
            res += c;
        }
        else if (c == ')') {
            while (!arr.empty() && arr.back() != '(') {
                res += arr.back();
                arr.pop_back();
            }
            if (!arr.empty()) {
                arr.pop_back();
            }
        }
        else if (c == '(') {
            arr.push_back(c);
        }
        else if (arr.empty() || arr.back() == '(' || precedence(c, arr.back())) {
            arr.push_back(c);
        }
        else {
            while (!arr.empty() && arr.back() != '(' && !precedence(c, arr.back())) {
                res += arr.back();
                arr.pop_back();
            }
            arr.push_back(c);
        }
    }

    while (!arr.empty()) {
        res += arr.back();
        arr.pop_back();
    }

    return res;
}

string polish_regex(const string& regex) {
    string reg = add_concat(regex);
    cout << reg << endl;
    string regg = compute_postfix(reg);
    return regg;
}