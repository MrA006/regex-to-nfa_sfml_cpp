#include "RegexToNfa.h"
#include "convertRegexToPostFix.h"
#include "Arrange.h"
#include "GUI.h"

int main() {
    
    string a;
    cout << "Enter regex : ";
    cin >> a;
    a = polish_regex(a);
    ETree* root = make_exp_tree(a);
    auto nfa = compute_regex(root);

    arrange_nfa(nfa);
    
    d.setn(::nfa.states.size());
    d.simulate();
    //cout << "Printing NFA:" << endl;
    //print_nfa(nfa.first);
    return 0;
}
