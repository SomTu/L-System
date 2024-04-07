// command line generator

#include <iostream>
#include <string>
#include <vector>
#include <optional>

struct Rule {
    std::string init;
    std::string fin;
};
std::ostream& operator<<(std::ostream& os, const Rule& rule) {
        os << rule.init << "->"  << rule.fin;
        return os;
}

const std::vector<char> allowed_rule_format_characters = {'-', '>', '(', ')'};

bool is_in_string(const std::string& str, const char& c) {
    for (char ch : str) {
        if (ch == c) return true;
    }
    return false;
}
template <typename T> bool is_in_vector(const std::vector<T>& vec, const T& element) {
    for (T e : vec) {
        if (e == element) {
            return true;
        }
    }
    return false;
}
bool check_letters_for_alphabet(const std::string& alphabet, const std::string& word_to_check, bool check_rule_format, const std::string& message) { //
    for (char letter : word_to_check) {
        if (!is_in_string(alphabet, letter)) {
            if (check_rule_format == true) {
                if (is_in_vector(allowed_rule_format_characters, letter)) {
                    continue;
                }
            }
            std::cout << message;
            return false;
        }
    }
    return true;
}
template <typename T>
void print_vec(const std::vector<T>& vec, const std::string& append = "") {
    for (T elem : vec) {
        std::cout << elem << append;
    }
}
bool check_parentheses(const std::string& rule) {
    int parentheses_count = 0;
    for (int i = 0; i < rule.size(); i++) {
        if (rule.at(i) == '(') parentheses_count++;
        if (rule.at(i) == ')') parentheses_count--;
        if (parentheses_count < 0) {
            std::cout << "Wrong parentheses in \"" << rule << "\"\n";
            return false;
        }
    }
    if (parentheses_count != 0) {
        std::cout << "Wrong parentheses placement int \"" << rule << "\"\n";
        return false;
    }
    return true;
}
bool check_rule_format(const std::string& rule) {
    // format is xyz->XYZ where
    // xyz are letters from alphabet
    // '-' and '>' are formal characters for rule recognition
    // XYZ are letters from alphabet or '(' or ')'
    // the three above are already checked for appearance as single letters
    // need to check -
    //      '-' is there exactly once
    //      '>' is there exactly once and is right after '-'
    //      '(' and ')' are only after '>' and they are properly situated

    int pos_dash = rule.find('-');
    int pos_arrow = rule.find('>');
    if (pos_arrow != pos_dash + 1) { // the arrow isn't correct
        std::cout << "Arrow not formatted correctly in rule \"" << rule << "\"\n";
        return false;
    }
    if (pos_arrow >= rule.size() - 1) { // there are no chars after ->
        std::cout << "No final part of rule \"" << rule << "\"\n";
        return false;
    }
    if (pos_dash < 1) { // there are no chars before ->
        std::cout << "No initial part of rule \"" << rule << "\"\n";
        return false;
    }
    if (pos_dash > 1) { 
        std::cout << "For now you cannot use more symbols in initial part of rule \"" << rule << "\"\n";
        return false;
    }
    if (rule.find('(') < pos_dash || rule.find(')') < pos_dash) { // there are parentheses before the arrow
        std::cout << "No parentheses allowed in initial part of rule \"" << rule << "\"\n";
        return false;
    }
    if (!check_parentheses(rule)) { // parentheses placed incorrectly
        // error message handled in called function
        return false;
    }


    return true;
}
Rule get_rule(const std::string& rule) {
    Rule _rule;
    if (!check_rule_format(rule)) {
        std::cout << "Wrong rule format.\n";
        return _rule;
    }
    std::string init = rule.substr(0, rule.find('-'));
    std::string fin = rule.substr(rule.find('>') + 1, rule.size());
    _rule.init = init;
    _rule.fin = fin;
    return _rule;
}
bool initiate_variables(const int& argc, char**& argv, int& iterations, std::string& alphabet, std::string& axiom, std::vector<Rule>& rules) {
    // argv[1] - number or iterations
    // argv[2] - alphabet
    // argv[3] - axiom
    // argv[4+] - rules in form xzy->XYZ
    if (argc < 4) {
        std::cout << "Not enough parameters. Parameters are [(int)number of iterations] [alphabet] [axiom] [rules...]\n";
        return false;
    }

    // initiate iteration count
    try {
        iterations = std::stoi(argv[1]);
    } catch (const std::exception& e) {
        std::cout << e.what() << " no conversion to int\n";
        return false;
    }

    // initiate alphabet
    alphabet = argv[2];
    for (char c : alphabet) {
        if (c == '-' || c == '>') {
            std::cout << "Alphabet cannot contain chars \'-\' or \'>\'.\n";
            return false;
        }
    }

    axiom = argv[3];
    if (!check_letters_for_alphabet(alphabet, axiom, false, "Axiom contains chars that are not in aplphabet.\n")) {
        return false;
    }
    for (int i = 4; i < argc; i++) {
        std::string message = "Rule ";
        message.append(std::to_string(i - 4));
        message.append(" (\"");
        message.append(argv[i]);
        message.append("\") contains chars that are not in alphabet.\n");
        std::string rule = argv[i];
        if (!check_letters_for_alphabet(alphabet, rule, true, message)) {
            return false;
        }
        if (!check_rule_format(rule)) {
            std::cout << "Rule " << std::to_string(i - 4) << " (\"" << rule << "\") isn't correctly formatted.\n";
            return false;
        }
        rules.push_back(get_rule(rule));
    }
    return true;
}
std::optional<Rule> get_rule_of_char(char letter, const std::vector<Rule>& rules) {
    for (int i = 0; i < rules.size(); i++) {
        if (rules.at(i).init.at(0) == letter) { 
            return rules.at(i);
        }
    }
    return std::nullopt;
}
std::string iterate(const std::string& source, const std::vector<Rule>& rules) {
    std::string new_word = "";
    for (int i = 0; i < source.size(); i++) {
        char letter = source.at(i);
        std::optional<Rule> opt_rule = get_rule_of_char(letter, rules);
        if (opt_rule.has_value() == false) {
            new_word.push_back(letter);
            continue;
        }
        new_word.append(opt_rule.value().fin);
    }

    return new_word;
}


int main(int argc, char** argv) {
    // argv[1] - number or iterations
    // argv[2] - alphabet
    // argv[3] - axiom
    // argv[4+] - rules in form xzy->XYZ
    int iterations = 0;
    std::string alphabet = "";
    std::string axiom = "";
    std::vector<Rule> rules;
    if (!initiate_variables(argc, argv, iterations, alphabet, axiom, rules)) {
        return 0;
    }
    std::cout << "Application assuming only one letter initial parts of rules.\n";
    std::cout << "Iterations: " << iterations << "\nAlphabet: " << alphabet << "\nAxiom: " << axiom << "\n";
    print_vec(rules, " ");

    for (int i = 0; i < iterations; i++) {
        axiom = iterate(axiom, rules);
        std::cout << "\n" << axiom;
    }
    

    return 0;
}
