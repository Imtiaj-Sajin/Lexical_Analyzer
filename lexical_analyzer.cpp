#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "preprocessor.cpp"

using namespace std;

vector<string> invalidKeywords;
vector<string> invalidIdentifiers;
unordered_set<string> keywords;
unordered_set<string> identifiers;
unordered_set<char> punctuations;
unordered_set<char> operators;
vector<string> literalValues; // Renamed from literals

bool isKeyword(const string& str) {
    static const unordered_set<string> keywordSet = {"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval", "constexpr", "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "reflexpr", "register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"};

    return keywordSet.find(str) != keywordSet.end();
}

bool findIdentifier(const string& s) {
    if (s.empty()) return false;
    bool inString = false;
    for (size_t i = 0; i < s.length(); ++i) {
        char c = s[i];
        if (i == 0) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '#') {
                continue;
            } else {
                if (c == '"') {
                    inString = true;
                } else {
                    return false;
                }
            }
        } else {
            if (inString) {
                if (c == '"') {
                    if (i == s.length() - 1) {
                        inString = false;
                    }
                }
            } else {
                if (!isalnum(c) && c != '_') {
                    return false;
                }
            }
        }
    }
    return !inString;
}

void tokenizeLine(const string& line) {
    string token;
    bool inString = false;
    for (char c : line) {
        if (isspace(c) || ispunct(c)) {
            if (inString) {
                token += c;
            } else {
                if (!token.empty()) {
                    if (isKeyword(token)) {
                        keywords.insert(token);
                    } else if (findIdentifier(token)) {
                        identifiers.insert(token);
                    } else {
                        if (!token.empty()) {
                            literalValues.push_back(token);
                        }
                    }
                    token.clear();
                }
                if (ispunct(c)) {
                    punctuations.insert(c);
                }
            }
        } else {
            if (c == '"') {
                if (inString) {
                    inString = false;
                    token += c;
                    if (!token.empty()) {
                        literalValues.push_back(token);
                    }
                    token.clear();
                } else {
                    inString = true;
                    token += c;
                }
            } else {
                token += c;
            }
        }
    }
}

void checkForErrors(const string& line) {
    size_t pos;
    if ((pos = line.find("itn")) != string::npos) {
        invalidKeywords.push_back("itn");
    }
    if ((pos = line.find("#")) != string::npos) {
        size_t start = pos + 1;
        while (start < line.size() && isalnum(line[start])) {
            ++start;
        }
        invalidIdentifiers.push_back(line.substr(pos, start - pos));
    }
}

void findOperators(const string& s) {
    for (char c : s) {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>') {
            operators.insert(c);
        }
    }
}

void printReport() {
    cout << endl;
    cout << "~~~~~~~~~ Lexical Analyzer ~~~~~~~~~~~~" << endl;
    cout << endl;
    cout << "Invalid identifiers and keywords:" << endl;
    for (const auto& invalidKeyword : invalidKeywords) {
        cout << invalidKeyword << ", ";
    }
    for (const auto& invalidIdentifier : invalidIdentifiers) {
        cout << invalidIdentifier << ", ";
    }
    cout << endl;
    cout << "------------" << endl;

    cout << "Keywords:" << endl;
    for (const auto& keyword : keywords) {
        cout << keyword << ",";
    }
    cout << endl;
    cout << endl;

    cout << "Identifiers:" << endl;
    for (const auto& identifier : identifiers) {
        cout << identifier << ",";
    }
    cout << endl;
    cout << endl;

    cout << "Operators:" << endl;
    for (const auto& op : operators) {
        cout << op << ",";
    }
    cout << endl;
    cout << endl;

    cout << "Punctuations:" << endl;
    for (const auto& punctuation : punctuations) {
        cout << punctuation << ",";
    }
    cout << endl;
    cout << endl;

    cout << "Literals:" << endl;
    for (const auto& literal : literalValues) {
        cout << literal << ",";
    }
    cout << endl;
}

int main() {
    string inputFilePath = "lex_input.txt";
    preprocess(inputFilePath);

    ifstream file(inputFilePath);
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        tokenizeLine(line);
        checkForErrors(line);
        findOperators(line);
    }

    file.close();

    printReport();

    return 0;
}
