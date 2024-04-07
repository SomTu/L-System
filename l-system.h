#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <exception>
#include <functional>
//#include <QApplications>

template <typename T, class Container>
bool is_in(const T& element, const Container& c) { // requires `.at(int index)` and `.size()` methods
    // it should be possible to check, but I don't know how yet
    for (int i = 0; i < c.size(); i++) {
        if (c.at(i) == element) {
            return true;
        }
    }
    return false;
}

template <typename T>
void clear_stack(std::stack<T>& s) {
    while (!s.empty()) {
        s.pop();
    }
}

struct Rule {
    char pivot = (char)0;
    std::function<std::string()> change_to = nullptr;
};
struct Bracket {
    char open = (char)0;
    char close = (char)0;
};

bool check_bracket_validity(const Bracket& b, const std::vector<Bracket>& other_brackets) {
    char fr{b.open}, bc{b.close};
    if ((int)b.open == 0 || (int)b.close == 0) {
        return false;
    }
    for(int i = 0; i < other_brackets.size(); i++) {
        char ofr{other_brackets.at(i).open}, obc{other_brackets.at(i).close};
        if (fr == ofr && bc == obc) {
            continue;
        }
        if (fr == ofr ||
            fr == obc ||
            bc == ofr ||
            bc == obc) {
            return false;
        }
    }
    return true;
}
std::ostream& operator<<(std::ostream& os, const Bracket& b) {
    os << b.open << b.close;
    return os;
}

bool check_rule_validity(const Rule& rule, const std::vector<Bracket>& allowed_brackets) {
    if ((int)rule.pivot == 0 || rule.change_to == nullptr || rule.change_to().size() < 1) {
        return false;
    }
    for (int b = 0; b < allowed_brackets.size(); b++) {
        char o{allowed_brackets.at(b).open}, c{allowed_brackets.at(b).close};
        if (rule.pivot == o || rule.pivot == c) {
            return false;
        }
    }
    for (int i = 0; i < allowed_brackets.size(); i++) {
        if (!check_bracket_validity(allowed_brackets.at(i), allowed_brackets)) {
            std::cout << "Bracket " << allowed_brackets.at(i) << " is invalid\n";
            return false;
        }
    }
    std::string rule_end_string = rule.change_to();
    for (int b = 0; b < allowed_brackets.size(); b++) { //brackets format
        int parentheses_count = 0;
        for (int i = 0; i < rule_end_string.size(); i++) {
            if (rule_end_string.at(i) == allowed_brackets.at(b).open) parentheses_count++;
            if (rule_end_string.at(i) == allowed_brackets.at(b).close) parentheses_count--;
            if (parentheses_count < 0) {
                return false;
            }
        }
        if (parentheses_count != 0) {
            return false;
        }
    }
    return true;
}
std::ostream& operator<<(std::ostream& os, const Rule& rule) {
    os << rule.pivot << "->" << rule.change_to();
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
    for (int i = 0; i < vector.size(); i++) {
        os << vector.at(i);
        if (i < vector.size() - 1) {
            os << " ";
        }
    }
    return os;
}




bool do_l_system_input_check(
    const std::string& axiom,
    const std::vector<Rule>& rules,
    const std::vector<Bracket>& allowed_brackets) {
    if (axiom.size() == 0) {
        std::cout << "Please set axiom.\n";
        return false;
    }
    for (int i = 0; i < rules.size(); i++) {
        if (!check_rule_validity(rules.at(i), allowed_brackets)) {
            std::cout << "Rule " << rules.at(i) << " number " << i << " is invalid\n";
            return false;
        }
    }
    for (int i = 0; i < allowed_brackets.size(); i++) {
        if (!check_bracket_validity(allowed_brackets.at(i), allowed_brackets)) {
            std::cout << "Bracket pair " << allowed_brackets.at(i) << " is invalid\n";
            return false;
        }
    }
    std::cout 
    << "L-system with axiom \"" 
    << axiom 
    << "\" with ";
    if (rules.size() == 0) {
        std::cout << "no rules \n";
    } else {
        std::cout 
        << "rule"
        << (rules.size() > 1 ? "s " : " ")
        << rules;
    }
    if (allowed_brackets.size() == 0) {
        std::cout
        << " and no brackets.\n";
    } else {
        std::cout
        << " and bracket pair"
        << (allowed_brackets.size() > 1 ? "s " : " ")
        << allowed_brackets
        << ".\n";
    }
    return true;
}
bool set_rule_pivots(std::vector<char>& target, const std::vector<Rule>& rules) {
    for (int i = 0; i < rules.size(); i++) {
        char pivot = rules.at(i).pivot;
        if (!is_in(pivot, target)) {
            target.push_back(pivot);
        } else {
            return false;
        }
    }
    return true;
}

struct String_Iterator {
    std::string word;
    int index;
};
void create_string_iterator(String_Iterator& SI, std::string word) {
    SI.word = word;
    SI.index = 0;
}


class L_System /*: QObject*/ {
    //Q_OBJECT
    private:
    std::vector<Rule> rules;
    std::vector<Bracket> allowed_brackets;
    std::string axiom;
    std::vector<char> rule_pivots;
    std::stack<String_Iterator> it_stack;
    void advance_iteration(int iterations);
    void call_action_signal_on_string(const std::string& word, int distance_from_end) {
        for (auto c : word) {
            call_action_signal(c, distance_from_end);
        }
    }
    void call_action_signal(char letter, int distance_from_end) {
        switch (letter) {
            // Qt signals
            default:
                std::cout << letter;
                break;
        }
    }
    public:
    void run(int iterations);
    L_System(
        const std::string& axiom,
        std::vector<Rule>& rules,
        std::vector<Bracket>& allowed_brackets) {
            if (!do_l_system_input_check(axiom, rules, allowed_brackets)) { 
                return; 
            }
            if (!set_rule_pivots(rule_pivots, rules)) {
                std::cout << "Some rules have duplicate pivots!\n";
                return;
            }
            this->rules = rules;
            this->axiom = axiom;
            this->allowed_brackets = allowed_brackets;
            this->it_stack = std::stack<String_Iterator>();
            
    }
    
};

void L_System::advance_iteration(int iterations) {
    String_Iterator& it = it_stack.top();
    std::string& word = it.word;
    int& index = it.index;
    if (index >= word.size()) { // >= just for caution, should be unnecessary
        it_stack.pop();
        return;
    }
    char current_char = word.at(index);
    if (it_stack.size() > iterations) {
        call_action_signal_on_string(word, );
        it_stack.pop();
        return;
    }
    if (!is_in(current_char, rule_pivots)) {
        call_action_signal(current_char, );
    } else {
        // find rule and push it onto stack
        for (Rule& rule : rules) {
            if (rule.pivot == current_char) {
                String_Iterator new_iteration_it;
                create_string_iterator(new_iteration_it, rule.change_to());
                it_stack.push(new_iteration_it);
                break;
            }
        }
    }
    ++index;
}
void L_System::run(int iterations) {
    clear_stack(it_stack);
    // axiom checked in constructor to be non-empty
    String_Iterator axiom_iterator;
    create_string_iterator(axiom_iterator, axiom);
    it_stack.push(axiom_iterator);
    while (!it_stack.empty()) {
        //std::cout << "\n" << it_stack.top().index << " ";
        advance_iteration(iterations);
    }
}