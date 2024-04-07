#include "l-system.h"



int main(int argc, char** argv) {
    Rule r, rr;
    r.pivot = 'F';
    r.change_to = [](){return "F+F--F+F";};
    std::vector<Rule> rules;
    rules.push_back(r);
    Bracket b{'(', ')'};
    std::vector<Bracket> brackets;
    brackets.push_back(b);
    L_System l("F", rules, brackets);
    l.run(3);
    return 0;
}